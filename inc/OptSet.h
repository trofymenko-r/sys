/*
 * OptSet.h
 *
 *  Created on: 23 трав. 2017
 *      Author: root
 */

#pragma once

#include "cmdtypes.h"

namespace cmd {

class COptSet
{
public:
   COptSet(EArgType ArgType, EValueType ValueType, EMandatoryType MandatoryType);
   COptSet(EArgType ArgType, EMandatoryType MandatoryType);
   COptSet(EArgType ArgType, EValueType ValueType);
   COptSet(EArgType ArgType);

   virtual ~COptSet();

public:
   EArgType ArgType;
   EMandatoryType MandatoryType;
   EValueType ValueType;
};

} /* namespace cross */
