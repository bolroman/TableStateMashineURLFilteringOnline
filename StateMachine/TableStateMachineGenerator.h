/*
 * TableStateMachineGenerator.h
 *
 *  Created on: Jan 25, 2011
 *      Author: yotamhc
 */

#ifndef TABLESTATEMACHINEGENERATOR_H_
#define TABLESTATEMACHINEGENERATOR_H_

#include "TableStateMachine.h"

TableStateMachine *generateTableStateMachine(Anchor *path, int numOfAnchors, int verbose);

#endif /* TABLESTATEMACHINEGENERATOR_H_ */
