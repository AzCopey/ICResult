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
	/// A simple alternate to checked exceptions for applications where exceptions would not
	/// be appropriate. 
	/// 
	/// This attempts to solve two problems. First of all it removes the requirement to
	/// supply output parameters from all methods which return a value as well as an error.
	/// For example:
	///
	///     bool tryGetValue(float& out_value);
	///
	/// Secondly it reduces the loss of context that often occurs in methods like this. When 
	/// false is returned an error presumably occurred internally, but we have no access to
	/// what caused it. 
	///
	/// Some systems may print to the console the internal error, however this becomes a 
	/// problem when the consumer of of the method gracefully handles the error. The printed 
	/// internal error is now no longer relevant and becomes spam. It can obscure other
	/// genuine errors.
	///
	/// To resolve these issues a Result can be used. This provides the means to return either
	/// an output value or an error. 
	///
	///     Result<float, ErrorEnum> tryGetValue();
	///
	/// In the case of an error a description can be provided, reducing loss of context. In 
	/// addition, if the error was caused by another, it can also be returned through the 
	/// Result. The user can then use getFullErrorMessage() to print the current error 
	/// descripion and, recursively, the errors that caused it.
	///
	/// For convenience, BoolResult has been provided which uses a boolean for the error
	/// type.
	///
	///     BoolResult<float> tryGetValue();
	///
	template <typename TValue, typename TError, TError TErrorOkay = TError()> class Result final : public IResult
	{
	public:
		/// Creates a successful result with the given value.
		/// 
		/// @param in_value - The output value.
		///
		explicit Result(const TValue& in_value) noexcept;

		/// Creates a failed result with the given error and message.
		///
		/// @param in_error - The error that occurred.
		/// @param in_errorMessage - A description of the error that occurred.
		///
		Result(TError in_error, const std::string& in_errorMessage) noexcept;

		/// Creates a failed result with the given error, message and the result that caused the error.
		///
		/// @param in_error - The error that occurred.
		/// @param in_errorMessage - A description of the error that occurred.
		///
		Result(TError in_error, const std::string& in_errorMessage, const IResult& in_causedBy) noexcept;

		/// @param in_toCopy - The result which should be copied.
		///
		Result(const Result<TValue, TError, TErrorOkay>& in_toCopy) noexcept;

		/// @param in_toMove - The result which should be moved into this.
		///
		Result(Result<TValue, TError, TErrorOkay>&& in_toMove) noexcept;

		/// @param in_toCopy - The result which should be copied.
		///
		Result<TValue, TError, TErrorOkay>& operator=(const Result<TValue, TError, TErrorOkay>& in_toCopy) noexcept;

		/// @param in_toMove - The result which should be moved into this.
		///
		Result<TValue, TError, TErrorOkay>& operator=(Result<TValue, TError, TErrorOkay>&& in_toMove) noexcept;

		/// Whether or not the result describes an error case. Typically this isn't called 
		/// directly, instead the IResult can be treated as a boolean, for example: 
		///
		///     if (result) {...}
		///
		/// This must be checked prior to getting the value.
		///
		/// @return Whether or not the result describes an error case. 
		///
		bool isOkay() const noexcept override;

		/// Before calling this isOkay() should be checked to ensure the result was successful
		/// therefore has a value.
		///
		/// @return The value.
		///
		const TValue& getValue() const noexcept;

		/// @return The error that occurred.
		///
		TError getError() const noexcept;

		/// @return A message describing the error. If not error occurred this will return
		/// an empty string.
		///
		const std::string& getErrorMessage() const noexcept override;

		/// @return A message describing this error and any errors which caused this error
		/// to occur. In other words, the output contains the error message for this and
		/// appends the full error message of the output from getCausedBy(). If no error
		/// occured this will return an empty string.
		///
		std::string getFullErrorMessage() const noexcept override;

		/// @return A pointer to the error which caused this one to occur. If this is not
		/// an error, or it wasn't caused by another this will return null.
		///
		const IResult* getCausedBy() const noexcept override;

		/// This is used internally to allow different derrived classes to clone another
		/// and therefore should be called rarely by the user of the class.
		///
		std::unique_ptr<const IResult> clone() const noexcept override;

	private:
		TValue m_value;
		TError m_error;
		std::string m_errorMessage;
		std::unique_ptr<const IResult> m_causedBy;
	};

	/// A convenience typedef for results which use a boolean error value.
	///
	template <typename TValue> using BoolResult = Result<TValue, bool, true>;
}

#include "ResultImpl.h"

#endif