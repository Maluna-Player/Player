/*************************************
 * @file    ArrayAccesException.h
 * @date    04/08/14
 * @author  Manuel
 *
 * Classe ArrayAccesException instanciée
 * lors de tentative d'accès en dehors des
 * limites d'un tableau.
 *************************************
*/

#ifndef __ARRAYACCESSEXCEPTION_H__
#define __ARRAYACCESSEXCEPTION_H__

#include "BaseException.h"
#include <sstream>


namespace exceptions {


class ArrayAccessException : public BaseException
{
    private:

        unsigned int m_ArraySize;

        int m_Pos;

    protected:

        virtual void buildMessage() override
        {
            std::ostringstream error;
            error << "Warning ! Out of range : Try to access to pos " << m_Pos << " of an array [0-" << m_ArraySize << "[";

            m_Error = error.str();
            BaseException::buildMessage();
        }

  public:

        ArrayAccessException(const std::string& method, unsigned int arraySize, int pos)
            : BaseException(method), m_ArraySize(arraySize), m_Pos(pos)
        {
            buildMessage();
        }

        virtual ~ArrayAccessException() throw()
        {

        }
};


} // exceptions

#endif  // __ARRAYACCESSEXCEPTION_H__
