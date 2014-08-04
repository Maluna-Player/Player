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
#include <sstream>

class ArrayAccesException : public BaseException
{
  private:

    unsigned int m_ArraySize;

    int m_Pos;

  protected:

    virtual void buildMessage()
    {
      std::ostringstream error;
      error << "Warning ! Out of range : Try to access to pos " << m_Pos << " of an array [0-" << m_ArraySize-1 << "]";

      m_Error = error.str();
      BaseException::buildMessage();
    }

  public:

    ArrayAccesException(const std::string& method, unsigned int arraySize, int pos)
      : BaseException(method), m_ArraySize(arraySize), m_Pos(pos)
    {
      buildMessage();
    }

    virtual ~ArrayAccesException() throw()
    {

    }
};

#endif  // __ARRAYACCESSEXCEPTION_HPP__
