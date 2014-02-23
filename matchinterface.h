#pragma once

#include <utility>
#include <cstdint>

namespace FSM {
	/**
	 * \brief Interface class for classis that may be used to match strings
	 *
	 * \param MatchDataType The type of input the class matches.
	 * \param AcceptNameType The type of the data returned on a match
	 * \param FailType The value to return on a match failure.
	 */
	template<
		typename MatchDataType,
		typename AcceptNameType,
		AcceptNameType FailType
	>	class MatchInterface {
	public:
		/**
		 * \brief The all important function call.
		 *
		 * \param input A string of data representing the input to be matched.
		 * \returns a std::pair containing: A value in AcceptNameType,
		 * 	implementation dependant, may just be Accept/Fail, or may provide
		 * 	further information; and a uint32_t indicating how many elements of the
		 * 	input were matched.
		 */
		virtual std::pair<AcceptNameType, uint32_t> Match(
				MatchDataType* input,
				uint32_t inputSize
		) = 0;
	};
}
