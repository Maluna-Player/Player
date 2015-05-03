/*************************************
 * @file    BaseException.h
 * @date    02/08/14
 * @author  Manuel
 *
 * Classe BaseException parente des
 * différentes exceptions du projet.
 *************************************
*/

#ifndef __BASEEXCEPTION_H__
#define __BASEEXCEPTION_H__

#include <string>
#include <exception>

class BaseException : public std::exception
{
  private:

    std::string m_Message;

    std::string m_Method;

  protected:

    std::string m_Error;

    virtual void buildMessage()
    {
      m_Message = "[From " + m_Method + "] " + m_Error;
    }

  public:

    BaseException(const std::string& method)
        : m_Method(method), m_Error("")
    {

    }

    BaseException(const std::string& method, const std::string& error)
        : m_Method(method), m_Error(error)
    {
      buildMessage();
    }

    virtual ~BaseException() throw()
    {

    }

    virtual const char* what() const throw()
    {
      return m_Message.c_str();
    }
};

#endif  // __BASEEXCEPTION_H__
