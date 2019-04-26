/* 
* File:   SingletonStatus.h
*/


#ifndef SINGLETON_STATUS_H
#define SINGLETON_STATUS_H

#ifndef MAKE_ERROR_CODE
    #define MAKE_ERROR_CODE(x) ((x) * -1)
#endif // MAKE_ERROR_CODE

#ifndef IS_SUCESS
    #define IS_SUCCESS(x) ((x) >= 0)
#endif // IS_SUCESS

#ifndef IS_ERROR
    #define IS_ERROR(x) ((x) < 0)
#endif // IS_ERROR

namespace SingletonStatus
{
    //// SUCCEEDED
    static const long SUCCESS                                   = 0;
    static const long S_SINGLETON_ALREADY_EXISTS                = 101;

    //// ERRORS
    static const long E_INVALID_PARAMETER                       = MAKE_ERROR_CODE(201);
    static const long E_INSTANCE_EXCEPTION                      = MAKE_ERROR_CODE(202);
    static const long E_INSTANCE_ERROR                          = MAKE_ERROR_CODE(203);
    static const long E_INSTANCE_DOES_NOT_EXIST                 = MAKE_ERROR_CODE(204);
};

#endif // SINGLETON_STATUS_H
