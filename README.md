ICResult
========

A simple alternate to checked exceptions for applications where exceptions would not
be appropriate. 

This attempts to solve two problems. First of all it removes the requirement to
supply output parameters from all methods which return a value as well as an error.
For example:

    bool tryGetValue(float& out_value);
	
Secondly it reduces the loss of context that often occurs in methods like this. When 
false is returned an error presumably occurred internally, but we have no access to
what caused it. 

Some systems may print to the console the internal error, however this becomes a 
problem when the consumer of of the method gracefully handles the error. The printed 
internal error is now no longer relevant and becomes spam. It can obscure other
genuine errors.

To resolve these issues a `Result` can be used. This provides the means to return either
an output value or an error. 

    Result<float, ErrorEnum> tryGetValue();
	
In the case of an error a description can be provided, reducing loss of context. In 
addition, if the error was caused by another, it can also be returned through the 
`Result`. The user can then use `getFullErrorMessage()` to print the current error 
descripion and, recursively, the errors that caused it.

For convenience, `BoolResult` has been provided which uses a boolean for the error
type.

    BoolResult<float> tryGetValue();
    
Code Example
------------

    #include <iostream>
    #include <time.h>
    #include "ICResult/Result.h"
    
    enum class OperationResult
    {
    	k_success,
    	k_failedOperation,
    	k_operationNotStarted
    };
    
    IC::BoolResult<float> tryCalcValue()
    {
    	if (rand() % 2 == 0)
    	{
    		return IC::BoolResult<float>(5.5f);
    	}
    
    	return IC::BoolResult<float>(false, "Could not calculate value.");
    }
    
    IC::Result<float, OperationResult> tryPerformOperation()
    {
    	if (rand() % 2 == 0)
    	{
    		auto result = tryCalcValue();
    		if (result)
    		{
    			return IC::Result<float, OperationResult>(result.getValue());
    		}
    
    		return IC::Result<float, OperationResult>(OperationResult::k_failedOperation, "The opperation could not be performed.", result);
    	}
    
    	return IC::Result<float, OperationResult>(OperationResult::k_operationNotStarted, "Failed to start opperation.");
    }
    
    int main()
    {
    	srand((unsigned int)time(0));
    
    	auto result = tryPerformOperation();
    	if (result)
    	{
    		std::cout << "The result of the opperation is: " << result.getValue();
    	}
    	else
    	{
    		std::cout << result.getFullErrorMessage();
    	}
    
    	int a;
    	std::cin >> a;
    }

