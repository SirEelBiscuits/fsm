#pragma once

#include "matchinterface.h"

namespace FSM {
	/**
	 * \brief Matches a range of characters
	 *
	 * \param AcceptNameType The type of data in Match(foo).fisrt
	 * \param ReturnState The value of data in Match(foo).first
	 */
	template<
		typename InputDataType,
		typename AcceptNameType,
		AcceptNameType ReturnState
	> class MatchCharacterRange
		: public MatchInterface<InputDataType, AcceptNameType, ReturnState> {
	public:
		MatchCharacterRange(InputDataType start, InputDataType end)
			: _matchRangeStart(start)
			, _matchRangeEnd(end)
			, _negate(false)
		{}

		MatchCharacterRange(InputDataType start, InputDataType end, bool negate)
			: _matchRangeStart(start)
			, _matchRangeEnd(end)
			, _negate(negate)
		{}

		/**
		 * \returns {ReturnState, 1} on a successful match, {ReturnState, 0}
		 * 	otherwise
		 */
		std::pair<AcceptNameType, uint32_t>
		Match(InputDataType* input, uint32_t inputSize) const override {
			if(inputSize == 0
				|| (
					(input[0] < _matchRangeStart || input[0] > _matchRangeEnd)
					^ _negate
				)
			)
				return {ReturnState, 0};
			return {ReturnState, 1};
		}

	private:
		InputDataType const _matchRangeStart;
		InputDataType const _matchRangeEnd;
		bool const _negate;
	};
}

