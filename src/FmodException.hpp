/*************************************
 * @file    FmodException.hpp
 * @date    02/08/14
 * @author  Manuel
 *
 * Classe FmodException instanciée
 * lors d'erreurs avec fonctions FMOD.
 *************************************
*/

#ifndef __FMODEXCEPTION_HPP__
#define __FMODEXCEPTION_HPP__

#include "BaseException.hpp"

class FmodException : public BaseException
{
  private:

    std::string m_FmodFunc;

    std::string m_FmodError;

  protected:

    virtual void buildMessage()
    {
      m_Error = m_FmodError + "(" + m_FmodFunc + ")";
      BaseException::buildMessage();
    }

  public:

    FmodException(const std::string& method, const std::string& fmodFunc, const char *error)
      : BaseException(method), m_FmodFunc(fmodFunc), m_FmodError(error)
    {
      buildMessage();
    }

    virtual ~FmodException() throw()
    {

    }
};

#endif  // __FMODEXCEPTION_HPP__
