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
	template <typename TError, TError TErrorSuccess> class Result<void, TError, TErrorSuccess> final : public IResult
	{
	public:
		//-----------------------------------------------------------------------------
		Result() noexcept
			: m_error(TErrorSuccess)
		{
		}

		//-----------------------------------------------------------------------------
		Result(TError in_error, const std::string& in_errorMessage) noexcept
			: m_error(in_error), m_errorMessage(in_errorMessage)
		{
			assert(!wasSuccessful());
		}

		//-----------------------------------------------------------------------------
		Result(TError in_error, const std::string& in_errorMessage, const IResult& in_causedBy) noexcept
			: m_errorMessage(in_errorMessage), m_causedBy(in_causedBy.cloneError())
		{
			assert(!wasSuccessful());
			assert(!m_causedBy);
		}

		//-----------------------------------------------------------------------------
		Result(const Result<void, TError, TErrorSuccess>& in_toCopy) noexcept
			: m_error(in_toCopy.m_error), m_errorMessage(in_toCopy.m_errorMessage), m_causedBy(in_toCopy.m_causedBy->cloneError())
		{
		}

		//-----------------------------------------------------------------------------
		Result(Result<void, TError, TErrorSuccess>&& in_toMove) noexcept
			: m_error(in_toMove.m_error), m_errorMessage(in_toMove.m_errorMessage), m_causedBy(std::move(in_toMove.m_causedBy))
		{
			in_toMove.m_errorMessage = "";
		}

		//-----------------------------------------------------------------------------
		Result<void, TError, TErrorSuccess>& operator=(const Result<void, TError, TErrorSuccess>& in_toCopy) noexcept
		{
			m_error = in_toCopy.m_error;
			m_errorMessage = in_toCopy.m_errorMessage;
			m_causedBy = in_toCopy.m_causedBy->clone();

			return *this;
		}

		//-----------------------------------------------------------------------------
		Result<void, TError, TErrorSuccess>& operator=(Result<void, TError, TErrorSuccess>&& in_toMove) noexcept
		{
			m_error = in_toMove.m_error;
			m_errorMessage = in_toMove.m_errorMessage;
			m_causedBy = std::move(in_toMove.m_causedBy);

			in_toMove.m_errorMessage = "";

			return *this;
		}

		//-----------------------------------------------------------------------------
		bool wasSuccessful() const noexcept override
		{
			return m_error == TErrorSuccess;
		}

		//-----------------------------------------------------------------------------
		void getValue() const noexcept
		{
			assert (false)
		}

		//-----------------------------------------------------------------------------
		TError getError() const noexcept
		{
			return m_error;
		}

		//-----------------------------------------------------------------------------
		const std::string& getErrorMessage() const noexcept override
		{
			assert(!wasSuccessful());

			return m_errorMessage;
		}

		//-----------------------------------------------------------------------------
		std::string getFullErrorMessage() const noexcept override
		{
			std::string errorMessage = m_errorMessage;
			if (m_causedBy)
			{
				errorMessage += "\nCaused by:\n" + m_causedBy->getFullErrorMessage();
			}
			return errorMessage;
		}

		//-----------------------------------------------------------------------------
		const IResult* getCausedBy() const noexcept override
		{
			assert(!wasSuccessful());

			return m_causedBy.get();
		}

		//-----------------------------------------------------------------------------
		std::unique_ptr<const IResult> cloneError() const noexcept override
		{
			assert(!wasSuccessful());

			if (m_causedBy)
			{
				return std::unique_ptr<const IResult>(new Result<void, TError, TErrorSuccess>(m_error, m_errorMessage, *m_causedBy));
			}
			else
			{
				return std::unique_ptr<const IResult>(new Result<void, TError, TErrorSuccess>(m_error, m_errorMessage));
			}
		}

	private:
		TError m_error;
		std::string m_errorMessage;
		std::unique_ptr<const IResult> m_causedBy;
	};

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorSuccess> Result<TValue, TError, TErrorSuccess>::Result(const TValue& in_value) noexcept
		: m_value(in_value), m_error(TErrorSuccess)
	{
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorSuccess> Result<TValue, TError, TErrorSuccess>::Result(TError in_error, const std::string& in_errorMessage) noexcept
		: m_error(in_error), m_errorMessage(in_errorMessage)
	{
		assert(!wasSuccessful());
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorSuccess> Result<TValue, TError, TErrorSuccess>::Result(TError in_error, const std::string& in_errorMessage, const IResult& in_causedBy) noexcept
		: m_error(in_error), m_errorMessage(in_errorMessage), m_causedBy(in_causedBy.cloneError())
	{
		assert(!wasSuccessful());
		assert(!m_causedBy->wasSuccessful());
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorSuccess> Result<TValue, TError, TErrorSuccess>::Result(const Result<TValue, TError, TErrorSuccess>& in_toCopy) noexcept
		: m_value(in_toCopy.m_value), m_error(in_toCopy.m_error), m_errorMessage(in_toCopy.m_errorMessage), m_causedBy(in_toCopy.m_causedBy->cloneError())
	{
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorSuccess> Result<TValue, TError, TErrorSuccess>::Result(Result<TValue, TError, TErrorSuccess>&& in_toMove) noexcept
		: m_value(in_toMove.m_value), m_error(in_toMove.m_error), m_errorMessage(in_toMove.m_errorMessage), m_causedBy(std::move(in_toMove.m_causedBy))
	{
		in_toMove.m_errorMessage = "";
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorSuccess> Result<TValue, TError, TErrorSuccess>& Result<TValue, TError, TErrorSuccess>::operator=(const Result<TValue, TError, TErrorSuccess>& in_toCopy) noexcept
	{
		m_value = in_toCopy.m_value;
		m_error = in_toCopy.m_error;
		m_errorMessage = in_toCopy.m_errorMessage;
		m_causedBy = in_toCopy.m_causedBy->clone();

		return *this;
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorSuccess> Result<TValue, TError, TErrorSuccess>& Result<TValue, TError, TErrorSuccess>::operator=(Result<TValue, TError, TErrorSuccess>&& in_toMove) noexcept
	{
		m_value = in_toMove.m_value;
		m_error = in_toMove.m_error;
		m_errorMessage = in_toMove.m_errorMessage;
		m_causedBy = std::move(in_toMove.m_causedBy);

		in_toMove.m_errorMessage = "";

		return *this;
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorSuccess> bool  Result<TValue, TError, TErrorSuccess>::wasSuccessful() const noexcept
	{
		return m_error == TErrorSuccess;
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorSuccess> const TValue&  Result<TValue, TError, TErrorSuccess>::getValue() const noexcept
	{
		assert(wasSuccessful());

		return m_value;
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorSuccess> TError  Result<TValue, TError, TErrorSuccess>::getError() const noexcept
	{
		return m_error;
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorSuccess> const std::string&  Result<TValue, TError, TErrorSuccess>::getErrorMessage() const noexcept
	{
		assert(!wasSuccessful());

		return m_errorMessage;
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorSuccess> std::string  Result<TValue, TError, TErrorSuccess>::getFullErrorMessage() const noexcept
	{
		assert(!wasSuccessful());

		std::string errorMessage = m_errorMessage;
		if (m_causedBy)
		{
			errorMessage += "\nCaused by:\n" + m_causedBy->getFullErrorMessage();
		}
		return errorMessage;
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorSuccess> const IResult*  Result<TValue, TError, TErrorSuccess>::getCausedBy() const noexcept
	{
		assert(!wasSuccessful());

		return m_causedBy.get();
	}

	//-----------------------------------------------------------------------------
	template <typename TValue, typename TError, TError TErrorSuccess> std::unique_ptr<const IResult>  Result<TValue, TError, TErrorSuccess>::cloneError() const noexcept
	{
		assert(!wasSuccessful());

		if (m_causedBy)
		{
			return std::unique_ptr<const IResult>(new Result<void, TError, TErrorSuccess>(m_error, m_errorMessage, *m_causedBy));
		}
		else
		{
			return std::unique_ptr<const IResult>(new Result<void, TError, TErrorSuccess>(m_error, m_errorMessage));
		}
	}
}

#endif