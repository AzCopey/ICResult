// Result.h
// Created by Ian Copland on 2015-07-18
//
// The MIT License(MIT)
// 
// Copyright(c) 2015 Ian Copland
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef _IC_RESULT_H_
#define _IC_RESULT_H_

#include "IResult.h"

namespace IC
{
	/// TODO
	///
	template <typename TValue, typename TError, TError TErrorOkay = TError()> class Result final : public IResult
	{
	public:
		/// TODO
		///
		Result(const TValue& in_value);

		/// TODO
		///
		Result(TError in_error, const std::string& in_errorMessage);

		/// TODO
		///
		Result(TError in_error, const std::string& in_errorMessage, const IResult& in_causedBy);

		/// TODO
		///
		Result(const Result<TValue, TError, TErrorOkay>& in_toCopy);

		/// TODO
		///
		Result(Result<TValue, TError, TErrorOkay>&& in_toMove);

		/// TODO
		///
		Result<TValue, TError, TErrorOkay>& operator=(const Result<TValue, TError, TErrorOkay>& in_toCopy);

		/// TODO
		///
		Result<TValue, TError, TErrorOkay>& operator=(Result<TValue, TError, TErrorOkay>&& in_toMove);

		/// TODO
		///
		bool isOkay() const override;

		/// TODO
		///
		const TValue& getValue() const;

		/// TODO
		///
		TError getError() const;

		/// TODO
		///
		const std::string& getErrorMessage() const override;

		/// TODO
		///
		std::string getFullErrorMessage() const override;

		/// TODO
		///
		const IResult* getCausedBy() const override;

		/// TODO
		///
		std::unique_ptr<const IResult> clone() const override;

	private:
		TValue m_value;
		TError m_error;
		std::string m_errorMessage;
		std::unique_ptr<const IResult> m_causedBy;
	};

	template <typename TValue> using BoolResult = Result<TValue, bool, true>;
}

#include "ResultImpl.h"

#endif