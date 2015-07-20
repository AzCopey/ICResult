// ResultImpl.h
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

#ifndef _IC_RESULTIMPL_H_
#define _IC_RESULTIMPL_H_

#include <assert.h>

#include "Result.h"

namespace IC
{
	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorOkay> Result<TValue, TError, TErrorOkay>::Result(const TValue& in_value) noexcept
		: m_value(in_value), m_error(TErrorOkay)
	{
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorOkay> Result<TValue, TError, TErrorOkay>::Result(TError in_error, const std::string& in_errorMessage) noexcept
		: m_error(in_error), m_errorMessage(in_errorMessage)
	{
		assert(m_error != TErrorOkay);
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorOkay> Result<TValue, TError, TErrorOkay>::Result(TError in_error, const std::string& in_errorMessage, const IResult& in_causedBy) noexcept
		: m_error(in_error), m_errorMessage(in_errorMessage), m_causedBy(in_causedBy.clone())
	{
		assert(m_error != TErrorOkay);
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorOkay> Result<TValue, TError, TErrorOkay>::Result(const Result<TValue, TError, TErrorOkay>& in_toCopy) noexcept
		: m_value(in_toCopy.m_value), m_error(in_toCopy.m_error), m_errorMessage(in_toCopy.m_errorMessage), m_causedBy(in_toCopy.m_causedBy->clone())
	{
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorOkay> Result<TValue, TError, TErrorOkay>::Result(Result<TValue, TError, TErrorOkay>&& in_toMove) noexcept
		: m_value(in_toMove.m_value), m_error(in_toMove.m_error), m_errorMessage(in_toMove.m_errorMessage), m_causedBy(std::move(in_toMove.m_causedBy))
	{
		in_toMove.m_errorMessage = "";
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorOkay> Result<TValue, TError, TErrorOkay>& Result<TValue, TError, TErrorOkay>::operator=(const Result<TValue, TError, TErrorOkay>& in_toCopy) noexcept
	{
		m_value = in_toCopy.m_value;
		m_error = in_toCopy.m_error;
		m_errorMessage = in_toCopy.m_errorMessage;
		m_causedBy = in_toCopy.m_causedBy->clone();
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorOkay> Result<TValue, TError, TErrorOkay>& Result<TValue, TError, TErrorOkay>::operator=(Result<TValue, TError, TErrorOkay>&& in_toMove) noexcept
	{
		m_value = in_toMove.m_value;
		m_error = in_toMove.m_error;
		m_errorMessage = in_toMove.m_errorMessage;
		m_causedBy = std::move(in_toMove.m_causedBy);

		in_toMove.m_errorMessage = "";
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorOkay> bool  Result<TValue, TError, TErrorOkay>::isOkay() const noexcept
	{
		return m_error == TErrorOkay;
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorOkay> const TValue&  Result<TValue, TError, TErrorOkay>::getValue() const noexcept
	{
		return m_value;
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorOkay> TError  Result<TValue, TError, TErrorOkay>::getError() const noexcept
	{
		return m_error;
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorOkay> const std::string&  Result<TValue, TError, TErrorOkay>::getErrorMessage() const noexcept
	{
		return m_errorMessage;
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorOkay> std::string  Result<TValue, TError, TErrorOkay>::getFullErrorMessage() const noexcept
	{
		std::string errorMessage = m_errorMessage;
		if (m_causedBy)
		{
			errorMessage += "\nCaused by:\n" + m_causedBy->getFullErrorMessage();
		}
		return errorMessage;
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorOkay> const IResult*  Result<TValue, TError, TErrorOkay>::getCausedBy() const noexcept
	{
		return m_causedBy.get();
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorOkay> std::unique_ptr<const IResult>  Result<TValue, TError, TErrorOkay>::clone() const noexcept
	{
		if (isOkay() == true)
		{
			return std::unique_ptr<const IResult>(new Result<TValue, TError, TErrorOkay>(m_value));
		}
		else if (m_causedBy)
		{
			return std::unique_ptr<const IResult>(new Result<TValue, TError, TErrorOkay>(m_error, m_errorMessage, *m_causedBy));
		}
		else
		{
			return std::unique_ptr<const IResult>(new Result<TValue, TError, TErrorOkay>(m_error, m_errorMessage));
		}
	}
}

#endif