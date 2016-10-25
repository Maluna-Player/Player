/*************************************
 * @file    LibException.h
 * @date    02/08/14
 * @author  Manuel
 *
 * Classe LibException instanciée
 * lors d'erreurs avec fonctions de librairies.
 *************************************
*/

#ifndef __LIBEXCEPTION_H__
#define __LIBEXCEPTION_H__

#include "BaseException.h"


namespace exceptions {


class LibException : public BaseException
{
    private:

        std::string m_LibFunc;

        std::string m_LibError;

    protected:

        virtual void buildMessage() override
        {
            m_Error = "'" + m_LibFunc + "' function produced the following error : " + m_LibError;
            BaseException::buildMessage();
        }

    public:

        LibException(const std::string& method, const std::string& libFunc, const char *error)
            : BaseException(method), m_LibFunc(libFunc), m_LibError(error)
        {
            buildMessage();
        }

        virtual ~LibException() throw()
        {

        }
};


} // exceptions

#endif  // __LIBEXCEPTION_H__
