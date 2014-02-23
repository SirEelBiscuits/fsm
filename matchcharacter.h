#pragma once

#include "matchinterface.h"

namespace FSM {
	/**
	 * \brief Matches a single character
	 *
	 * \param InputDataType Does what it says on the tin.
	 * \param AcceptNameType The type of data in Match(foo).first
	 * \param ReturnState The value of Match(foo).first
	 */
	template<
		typename InputDataType,
		typename AcceptNameType,
		AcceptNameType ReturnState
	> class MatchCharacter
		: public MatchInterface<InputDataType, AcceptNameType, ReturnState> {
	public:
		MatchCharacter(InputDataType i)
			: _matchCharacter(i)
		{}

		MatchCharacter(InputDataType i, bool negate)
			: _matchCharacter(i)
			, _negate(negate)
		{}

		/**
		 * \returns {ReturnState, 1} on a successful match, {ReturnState, 0}
		 * 	otherwise
		 */
		std::pair<AcceptNameType, uint32_t>
		Match(InputDataType* input, uint32_t inputSize) const override {
			if(inputSize == 0
					|| ((_matchCharacter != input[0]) ^ _negate))
				return {ReturnState, 0};
			return {ReturnState, 1};
		}

	private:
		InputDataType _matchCharacter;
		bool _negate;
	};
}

