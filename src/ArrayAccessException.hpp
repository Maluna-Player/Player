/*************************************
 * @file    ArrayAccesException.hpp
 * @date    04/08/14
 * @author  Manuel
 *
 * Classe ArrayAccesException instanciée
 * lors de tentative d'accès en dehors des
 * limites d'un tableau.
 *************************************
*/

#ifndef __ARRAYACCESSEXCEPTION_HPP__
#define __ARRAYACCESSEXCEPTION_HPP__

#include "BaseException.hpp"

class ArrayAccesException : public BaseException
{
  public:

    ArrayAccesException(const std::string& method)
      : BaseException(method, "Warning ! Try to access out of range of an array !")
    {

    }

    virtual ~ArrayAccesException() throw()
    {

    }
};

#endif  // __ARRAYACCESSEXCEPTION_HPP__
