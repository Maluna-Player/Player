/*************************************
 * @file    LibException.hpp
 * @date    02/08/14
 * @author  Manuel
 *
 * Classe LibException instanciée
 * lors d'erreurs avec fonctions de librairies.
 *************************************
*/

#ifndef __LIBEXCEPTION_HPP__
#define __LIBEXCEPTION_HPP__

#include "BaseException.hpp"

class LibException : public BaseException
{
  private:

    std::string m_LibFunc;

    std::string m_LibError;

  protected:

    virtual void buildMessage()
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

#endif  // __LIBEXCEPTION_HPP__
