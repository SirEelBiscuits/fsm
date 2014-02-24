#pragma once

#include <map>
#include <vector>
#include <utility>
#include <memory>

#include "matchinterface.h"
#include "matchcharacterrange.h"

namespace FSM {
	/**
	 * \brief Finite state machine for string matching
	 *
	 * A machine can be created that is equivalent to a regex, and then used
	 * 	to match against a string.
	 * Unlike a standard regex, this allows multiple end-points, so a single
	 * 	FSM can not only match very different things, but also let you know
	 * 	which one it matched.
	 *
	 * USAGE
	 * To create a state machine, it is recommended that you use a uniform
	 * 	initialiser like so:
	 *
	 * StateMachine<char, char, int, 0> fsm {
	 *  'a', //initial state
	 * 	{ //states
	 * 		{ 'a', { {MatchCharacter<char,int,0,1>('a'), {'b'}}}},
	 * 		{ 'b', {
	 * 		         {MatchCharacterRange<char,int,0,1>('a', 'z'), {'c'}}
	 * 		         {MatchCharacterRange<char,int,0,1>('A', 'Z'), {'d'}}
	 * 		       }
	 * 		}
	 * 	},
	 *  { //accept states
	 *    {'c', 1},
	 *    {'d', 2},
	 *  }
	 * };
	 * This is squivalent to the regex: "a[a-zA-Z]", except the return of
	 * 	Match will depend on whether the last character was in [a-z] or [A-Z]
	 *
	 * \todo Currently the AcceptanceNameType is required to have operator==
	 * 	overloaded for this class to work. In future a comparator should be added.
	 *
	 * \param FailState The state Match should return if no match is found.
	 * \param StateTransitionType The type of the data used as event input for
	 * 	the state machine
	 * \param StateNameType The type of the objects used to identify
	 * 	individual states. Recommended to be an enum class.
	 * \param AcceptanceNameType The type of the objects Match should return
	 * 	on successfully matching an input string.
	 */
	template<
		typename StateTransitionType,
		typename StateNameType,
		typename AcceptanceNameType,
		AcceptanceNameType FailState
	> class StateMachine
		: public MatchInterface<
			StateTransitionType,
		 	AcceptanceNameType,
		 	FailState
	>	{
	public:
		StateNameType InitialState;

		/**
		 * \brief Defines the input to trigger a transition.
		 */
		typedef std::shared_ptr<MatchInterface<
			StateTransitionType,
		 	AcceptanceNameType,
		 	FailState
		> const> MatchRange;

		/**
		 * \brief A Transition is a trigger, and a set of targets it leads to.
		 */
		typedef std::pair<MatchRange, std::vector<StateNameType>> Transition;

		/*
		 * \brief A state can literally be defined as the transitions it
		 * 	contains.
		 */
		typedef std::vector<Transition> State;

		/**
		 * \brief The actual state machine data
		 *
		 * This is the critical member of this class. It maps the templated
		 * 	StateNameType, onto the typedefed State. Typedefs are used for the
		 * 	inner types here to prevent polluting the uniform initialiser with
		 * 	errant-looking braces.
		 */
		std::map<StateNameType,	State> States;

		/**
		 * \brief Acceptance states. These do not need to be present in States.
		 *
		 * AcceptanceNameType can be any type, though I'd recommend enums or
		 * 	something actually useful. This is the value that will be returned
		 * 	as part of the return type of the Match function, to indicate what
		 * 	was matched.
		 */
		std::map<StateNameType, AcceptanceNameType> AcceptanceStates;

		StateMachine(
			StateNameType initState,
			std::map<StateNameType, State> states,
		 	std::map<StateNameType, AcceptanceNameType> accStates
		)
			: InitialState(initState)
			, States(states)
			, AcceptanceStates(accStates)
		{}

		/**
		 * \brief Function to Match (or not) a set of input data.
		 *
		 * \param input An array of StateTransitionType elements, which are
		 * 	treated one by one as the input events.
		 *
		 * \returns A pair containing the data value in the acceptance states,
		 * 	and an unsinged value indicating how many characters were matched
		 */
		virtual std::pair<AcceptanceNameType, uint32_t> Match(
			StateTransitionType* input,
			uint32_t inputSize
		) const override {
			return Match(input, inputSize, InitialState, 0);
		}

	private:
		/**
		 * \brief The inner recursive call of Match.
		 *
		 * \param input Pointer to string of input data (no special terminator).
		 * \param inputLength
		 * \param CurState The state that the machine should run from.
		 */
		std::pair<AcceptanceNameType,uint32_t> Match(
			StateTransitionType* input,
			uint32_t inputLength,
		 	StateNameType CurState,
			uint32_t charsMatched
		) const {
			auto stateIter = States.find(CurState);
			bool validState = stateIter != States.end();
			auto acState = AcceptanceStates.find(CurState);
			State const& currentState = stateIter->second;
			if(inputLength == 0
					|| (validState && currentState.size() == 0)
					|| (!validState && acState != AcceptanceStates.end())
			) {
				if(acState != AcceptanceStates.end()) {
					return {acState->second, charsMatched};
				}
				return {FailState, 0};
			}
			if(validState) {
				for(auto cur : currentState) {
					auto match = cur.first->Match(input, inputLength);
					if(match.first != FailState)
						continue;
					for(auto s : cur.second) {
						auto retVal = Match(
							input + match.second,
							inputLength - match.second,
							s,
							charsMatched + match.second
						);
						if(retVal.second != 0)
							return retVal;
					}
				}
			}
			return {FailState, 0};
		}
	};
}

