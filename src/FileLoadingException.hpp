/*************************************
 * @file    FileLoadingException.hpp
 * @date    02/08/14
 * @author  Manuel
 *
 * Classe FileLoadingException instanciée
 * lors d'erreurs d'ouverture de fichiers.
 *************************************
*/

#ifndef __FILELOADINGEXCEPTION_HPP__
#define __FILELOADINGEXCEPTION_HPP__

#include "BaseException.hpp"

class FileLoadingException : public BaseException
{
  private:

    std::string m_File;

  protected:

    virtual void buildMessage()
    {
      m_Error = "Cannot open '" + m_File + "'";
      BaseException::buildMessage();
    }

  public:

    FileLoadingException(const std::string& method, const std::string& file)
      : BaseException(method), m_File(file)
    {
      buildMessage();
    }

    virtual ~FileLoadingException() throw()
    {

    }
};

#endif  // __FILELOADINGEXCEPTION_HPP__
