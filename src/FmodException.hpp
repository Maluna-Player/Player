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
  public:

    FmodException(const std::string& method, const std::string& fmodFunc, const char *error)
      : BaseException(method, std::string(error) + "(" + fmodFunc + ")")
    {

    }

    virtual ~FmodException() throw()
    {

    }
};

#endif  // __FMODEXCEPTION_HPP__
