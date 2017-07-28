/**************************************************************************************************/

/*
* File: List_R.cpp
* Author: Miguel A. Garcia
* NetID: magarcia1
* Assignment #: 3
* Date: November/10/2014
*
* Description:
*/

/**************************************************************************************************/

#include <iostream>
#include <sstream>
#include "Graph.h"
#include "List_R.h"

#include "Hlsyn.h"

/**************************************************************************************************/

bool List_R(Graph& aGraph, int aLatency){
	vector<Component*> addSubs;
	vector<Component*> divMods;
	vector<Component*> muls;
	vector<Component*> logics;
	vector<Component*> scheduled; //components that are already scheduled
	vector<Component*> toSchedule;//components not yet scheduled
	vector<Component*> candidates;//components not yet scheduled

	Component* currComp;
	Component* head;
	int* addSub = new int(1);
	int* divMod = new int(1);
	int* mul = new int(1);
	int* logic = new int(1);
	int* addSubTemp = new int(-1);
	int* divModTemp = new int(-1);
	int* mulTemp = new int(-1);
	int* logicTemp = new int(-1);
	int* timeStep = new int(0);

	if (!ALAPSchedule(aGraph, aLatency)) {
		cout << "ALAP scheduling could not find a suitable schedule" << endl;
		return false;
	}
	timeStep = new int(1);

	head = aGraph.getComponent(0);
	head->setScheduled(0);
	head->setScheduledCompleted(0);
	//candidate operations are the successor elements inside the vector schedule 
	//In here, we are pushing the head to the schedule vector (This means we already scheduled the head)
	scheduled.push_back(aGraph.getComponent(0));

	for (int k = 0; k < aLatency; k++){
	
		candidates = DetermineCandOp(scheduled, timeStep);
		/**addSubTemp = *addSub;
		*divModTemp = *divMod;
		*mulTemp = *mul;
		*logicTemp = *logic;*/
		for (unsigned int i = 0; i < candidates.size(); i++){
			currComp = candidates.at(i);
			
				if ((currComp->getType() == "adder/subtractor") && checkGoodCandidate(scheduled, currComp)){
					currComp->setSlack(currComp->getScheduled() - *timeStep);
					addSubs.push_back(currComp);
				}
				else if ((currComp->getType() == "multiplier") && checkGoodCandidate(scheduled, currComp)){
					currComp->setSlack(currComp->getScheduled() - *timeStep);
					muls.push_back(currComp);
				}
				else if ((currComp->getType() == "divider/modulo") && checkGoodCandidate(scheduled, currComp)){
					currComp->setSlack(currComp->getScheduled() - *timeStep);
					divMods.push_back(currComp);
				}
				else if ((currComp->getType() == "logic") && checkGoodCandidate(scheduled, currComp)){
					currComp->setSlack(currComp->getScheduled() - *timeStep);
					logics.push_back(currComp);
				}
				////schedule zero slack operations 
				//ZeroSlackScheduling(addSubs, toSchedule, addSub, addSubTemp, timeStep);
				//ZeroSlackScheduling(divMods, toSchedule, divMod, divModTemp, timeStep);
				//ZeroSlackScheduling(muls, toSchedule, mul, mulTemp, timeStep);
				//ZeroSlackScheduling(logics, toSchedule, logic, logicTemp, timeStep);
				////schedule Candidate Operations requiring no additional resources
				//ScheduleAvailableOp(addSubs, toSchedule, addSub, addSubTemp, timeStep);
				//ScheduleAvailableOp(divMods, toSchedule, divMod, divModTemp, timeStep);
				//ScheduleAvailableOp(muls, toSchedule, mul, mulTemp, timeStep);
				//ScheduleAvailableOp(logics, toSchedule, logic, logicTemp, timeStep);
		}
		//schedule zero slack operations 
		ZeroSlackScheduling(addSubs, toSchedule, addSub, addSubTemp, timeStep);
		ZeroSlackScheduling(divMods, toSchedule, divMod, divModTemp, timeStep);
		ZeroSlackScheduling(muls, toSchedule, mul, mulTemp, timeStep);
		ZeroSlackScheduling(logics, toSchedule, logic, logicTemp, timeStep);
		//schedule Candidate Operations requiring no additional resources
		ScheduleAvailableOp(addSubs, toSchedule, addSub, addSubTemp, timeStep);
		ScheduleAvailableOp(divMods, toSchedule, divMod, divModTemp, timeStep);
		ScheduleAvailableOp(muls, toSchedule, mul, mulTemp, timeStep);
		ScheduleAvailableOp(logics, toSchedule, logic, logicTemp, timeStep);

		for (unsigned int i = 0; i < toSchedule.size(); i++){
			toSchedule.at(i)->decrementor();
			if (toSchedule.at(i)->getTimeToSchedule() == 0){
				toSchedule.at(i)->setScheduledCompleted(*timeStep);
				scheduled.push_back(toSchedule.at(i));

				/*if (toSchedule.at(i)->getType() == "adder/subtractor"){
					*addSubTemp = *addSubTemp + 1;
				}
				else if (toSchedule.at(i)->getType() == "multiplier"){
					*mulTemp = *mulTemp + 1;
				}
				else if (toSchedule.at(i)->getType() == "divider/modulo"){
					*divModTemp = *divModTemp + 1;
				}
				else if (toSchedule.at(i)->getType() == "logic"){
					*logicTemp = *logicTemp + 1;
				}*/
				toSchedule.erase(toSchedule.begin() + i);
				i = i - 1;
			}
		}
		

		*timeStep = *timeStep + 1;
	}


	return true;
}

void ZeroSlackScheduling(vector<Component*>& aComp, vector<Component*>& toSchedule, int* resourceAvailable, int* resourceAvailableTemp, int* timeStep){
	*resourceAvailableTemp = *resourceAvailable;

	for (unsigned int i = 0; i < aComp.size(); i++){
		if (aComp.at(i)->getSlack() == 0){
			if (*resourceAvailableTemp == 0){
				*resourceAvailable = *resourceAvailable + 1;
				//schedule it
				aComp.at(i)->setScheduled(*timeStep);
				toSchedule.push_back(aComp.at(i));
				aComp.erase(aComp.begin() + i);
			}
			else{
				*resourceAvailableTemp = *resourceAvailableTemp - 1;
				//schedule it
				aComp.at(i)->setScheduled(*timeStep);
				toSchedule.push_back(aComp.at(i));
				aComp.erase(aComp.begin() + i);
			}
		}
	}
	
}

void ScheduleAvailableOp(vector<Component*>& aComp, vector<Component*>& toSchedule, int* resourceAvailable, int* resourceAvailableTemp, int* timeStep){
	if (*resourceAvailableTemp == 0){
		return;
	}
	else{
		for (unsigned int i = 0; i < aComp.size(); i++){
			if (*resourceAvailableTemp != 0){
				*resourceAvailableTemp = *resourceAvailableTemp - 1;
				//schedule it
				aComp.at(i)->setScheduled(*timeStep);
				toSchedule.push_back(aComp.at(i));
				aComp.erase(aComp.begin() + i);
			}
			else{
				return;
			}
		}
	}
	return;
}

vector<Component*> DetermineCandOp(vector<Component*> scheduled, int* timeStep){
	vector<Component*> timeCandidates;
	vector<Component*> TrueCandidates;

	for (unsigned int i = 0; i < scheduled.size(); i++){
		if (*timeStep-1 == scheduled.at(i)->getScheduledCompleted()){
			timeCandidates.push_back(scheduled.at(i));
		}
	}
	for (unsigned int j = 0; j < timeCandidates.size(); j++){
		for (int k = 0; k < timeCandidates.at(j)->getSuccessorSize(); k++){
			int size = timeCandidates.at(j)->getSuccessorSize();
			TrueCandidates.push_back(timeCandidates.at(j)->getSuccessor(k));
		}
	}
	return TrueCandidates;
}

bool checkGoodCandidate(vector<Component*> scheduled, Component* currComp){
	int temp = 0;
	for (unsigned int i = 0; i < scheduled.size(); i++){
		for (int j = 0; j < currComp->getPredecessorSize(); j++){
			if (currComp->getPredecessor(j) == scheduled.at(i)){
				temp = temp + 1;
			}
		}
	}
	if (temp == currComp->getPredecessorSize()){
		return true;
	}
	else{
		return false;
	}
}
/**************************************************************************************************/