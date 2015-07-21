// IResult.h
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

#ifndef _IC_IRESULT_H_
#define _IC_IRESULT_H_

#include <memory>
#include <string>

namespace IC
{
	/// An interface for all Result types. This is required for erasure of the specific 
	/// templated type, allowing a result to store another with different template 
	/// parameters as the "cause" of the error.
	///
	class IResult
	{
	public:

		/// Bool conversion operator. Allows validity queries in the form:
		///
		///     if (result) {...}
		///
		/// This unfortunately needs to be implemented in the interface as VS2015 doesn't
		/// allow the use of explicit on virtual methods.
		///
		/// @return Whether or not the result describes an error case.
		///
		explicit operator bool() const noexcept { return wasSuccessful(); };

		/// Whether or not the result describes an error case. Typically this isn't called 
		/// directly, instead the IResult can be treated as a boolean, for example: 
		///
		///     if (result) {...}
		///
		/// @return Whether or not the result describes an error case. 
		///
		virtual bool wasSuccessful() const noexcept = 0;

		/// @return A message describing the error. This should not  be called if no error 
		/// occurred.
		///
		virtual const std::string& getErrorMessage() const noexcept = 0;

		/// @return A message describing this error and any errors which caused this error
		/// to occur. In other words, the output contains the error message for this and
		/// appends the full error message of the output from getCausedBy(). This should not
		/// be called if no error occurred. This will be evaluated each time the method is 
		/// called. This is to avoid upfront cost if it isn't used.
		///
		virtual std::string getFullErrorMessage() const noexcept = 0;

		/// @return A pointer to the error which caused this one to occur. If it wasn't caused
		/// by another this will return null. This should not be called if no error occurred.
		///
		virtual const IResult* getCausedBy() const noexcept = 0;

		/// This is used internally to allow different derrived classes to clone another
		/// and therefore should be called rarely by the user of the interface. This should 
		/// not be called if no error occurred.
		///
		/// @return A clone of this as a unique pointer.
		///
		virtual std::unique_ptr<const IResult> cloneError() const noexcept = 0;

		virtual ~IResult() noexcept {}
	};
}

#endif