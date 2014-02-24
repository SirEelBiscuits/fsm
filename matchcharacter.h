#pragma once

#include "matchinterface.h"

namespace FSM {
	/**
	 * \brief Matches a single character
	 *
	 * \param InputDataType Does what it says on the tin.
	 * \param AcceptNameType The type of data in Match(foo).first
	 * \param FailState The value to return in Match(foo).first on failed match.
	 * \param AcceptState The value to return in Match(foo).first on match.
	 */
	template<
		typename InputDataType,
		typename AcceptNameType,
		AcceptNameType FailState,
		AcceptNameType AcceptState
	> class MatchCharacter
		: public MatchInterface<InputDataType, AcceptNameType, FailState> {
	public:
		MatchCharacter(InputDataType i)
			: _matchCharacter(i)
		{}

		MatchCharacter(InputDataType i, bool negate)
			: _matchCharacter(i)
			, _negate(negate)
		{}

		/**
		 * \returns {AcceptState, 1} on a successful match, {FailState, 0}
		 * 	otherwise
		 */
		std::pair<AcceptNameType, uint32_t>
		Match(InputDataType* input, uint32_t inputSize) const override {
			if(inputSize == 0
					|| ((_matchCharacter != input[0]) ^ _negate))
				return {FailState, 0};
			return {AcceptState, 1};
		}

	private:
		InputDataType _matchCharacter;
		bool _negate;
	};
}

