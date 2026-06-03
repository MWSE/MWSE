#pragma once

#include "TES3Defines.h"

#include "Deque.h"
#include "NIPoint3.h"

namespace TES3 {
	struct PathPlannerNode {
		NI::Point3 position;
		NI::Point3 parentPosition;
		se::Deque<PathPlannerNode>* linkedNodeList;
		int fCost;
		int gCost;
		int hCost;
		int flags;

		PathPlannerNode() = delete;
		~PathPlannerNode() = delete;
	};
	static_assert(sizeof(PathPlannerNode) == 0x2C, "TES3::PathPlannerNode failed size validation");

	struct PathPlanner {
		se::Deque<NI::Point3> pathList; // 0x0, world-space waypoints.
		se::Deque<PathPlannerNode> closedList; // 0xC
		se::Deque<PathPlannerNode> openList; // 0x18
		se::Deque<PathPlannerNode> scratchList; // 0x24
		PathPlannerNode currentNode; // 0x30
		PathPlannerNode goalNode; // 0x5C
		PathPlannerNode startNode; // 0x88
		MobileActor* mobileActor; // 0xB4
		MobileActor* targetActor; // 0xB8
		int cellMinX; // 0xBC
		int cellMinY; // 0xC0
		int cellMaxX; // 0xC4
		int cellMaxY; // 0xC8
		PathGrid* pathGrid; // 0xCC
		bool inInterior; // 0xD0
		char padding_0xD1[3];
		int exteriorWrapOffsetX; // 0xD4
		int exteriorWrapOffsetY; // 0xD8
		int maybePreviousWrapOffsetX; // 0xDC
		int maybePreviousWrapOffsetY; // 0xE0
		NI::Point3 currentWaypoint; // 0xE4
		bool hasCurrentWaypoint; // 0xF0
		bool pathComplete; // 0xF1
		bool pathStarted; // 0xF2
		char padding_0xF3;

		PathPlanner() = delete;
		~PathPlanner() = delete;

		bool buildPath(int startX, int startY, int startZ, int destinationX, int destinationY, int destinationZ);
		void clearSearchStateAndPath();
	};
	static_assert(sizeof(PathPlanner) == 0xF4, "TES3::PathPlanner failed size validation");
}
