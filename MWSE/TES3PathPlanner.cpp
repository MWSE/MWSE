#include "TES3PathPlanner.h"

#include "TES3Cell.h"
#include "TES3Util.h"

namespace TES3 {
	namespace {
		constexpr auto MaxAttachmentDistance = 1024.0f;

		struct SearchNode {
			PathGrid::Node* pathGridNode;
			SearchNode* parent;
			int gCost;
			int fCost;
			bool closed;
			bool opened;
		};

		struct ReachableNodeCandidate {
			PathGrid::Node* pathGridNode;
			int cost;
		};

		int calculateExteriorWrapOffset(int coordinate) {
			return coordinate % Cell::exteriorGridWidth < 0 ? Cell::exteriorGridWidth : 0;
		}

		NI::Point3 toLocalPoint(const PathPlanner* planner, int x, int y, int z) {
			if (planner->inInterior) {
				return { float(x), float(y), float(z) };
			}

			return {
				float(planner->exteriorWrapOffsetX + x % Cell::exteriorGridWidth),
				float(planner->exteriorWrapOffsetY + y % Cell::exteriorGridWidth),
				float(z),
			};
		}

		NI::Point3 toWorldPoint(const PathPlanner* planner, const NI::Point3& point) {
			if (planner->inInterior) {
				return point;
			}

			return {
				point.x + float(planner->cellMinX),
				point.y + float(planner->cellMinY),
				point.z,
			};
		}

		SearchNode* findSearchNode(std::vector<SearchNode>& searchNodes, const PathGrid::Node* pathGridNode) {
			const auto it = std::find_if(searchNodes.begin(), searchNodes.end(),
				[pathGridNode](const auto& node) {
					return node.pathGridNode == pathGridNode;
				}
			);

			if (it == searchNodes.end()) {
				return nullptr;
			}

			return &*it;
		}

		PathGrid::Node* findClosestReachableNode(const PathPlanner* planner, const std::vector<PathGrid::Node*>& nodes, const NI::Point3& point) {
			const auto exactIt = std::find_if(nodes.begin(), nodes.end(),
				[&point](const auto node) {
					return node->getLocalPosition() == point;
				}
			);
			if (exactIt != nodes.end()) {
				return *exactIt;
			}

			std::vector<ReachableNodeCandidate> candidates;
			candidates.reserve(nodes.size());
			for (auto node : nodes) {
				auto nodePosition = node->getLocalPosition();
				candidates.push_back({ node, int(point.distanceManhattan(&nodePosition)) });
			}

			std::sort(candidates.begin(), candidates.end(), [](const auto& lhs, const auto& rhs) {
				return lhs.cost < rhs.cost;
				});

			auto worldPoint = toWorldPoint(planner, point);
			for (const auto& candidate : candidates) {
				auto candidatePosition = candidate.pathGridNode->getLocalPosition();
				if (point.distance(&candidatePosition) > MaxAttachmentDistance) {
					continue;
				}

				auto worldCandidatePosition = toWorldPoint(planner, candidatePosition);
				if (mwse::tes3::testLineOfSight(&worldPoint, 64.0f, &worldCandidatePosition, 64.0f)) {
					return candidate.pathGridNode;
				}
			}

			return nullptr;
		}
	}

	bool PathPlanner::buildPath(int startX, int startY, int startZ, int destinationX, int destinationY, int destinationZ) {
		clearSearchStateAndPath();

		if (!pathGrid || pathGrid->nodeCount == 0) {
			return false;
		}

		if (!inInterior) {
			if (destinationX < cellMinX || destinationY < cellMinY || destinationX > cellMaxX || destinationY > cellMaxY) {
				return false;
			}

			exteriorWrapOffsetX = calculateExteriorWrapOffset(startX);
			exteriorWrapOffsetY = calculateExteriorWrapOffset(startY);
		}

		std::vector<PathGrid::Node*> nodes;
		nodes.reserve(pathGrid->nodeCount);
		for (auto node : pathGrid->nodes) {
			if (node) {
				nodes.push_back(node);
			}
		}

		if (nodes.empty()) {
			return false;
		}

		const auto localStart = toLocalPoint(this, startX, startY, startZ);
		const auto localDestination = toLocalPoint(this, destinationX, destinationY, destinationZ);
		startNode.position = localStart;
		startNode.gCost = 0;
		startNode.hCost = int(localStart.distanceManhattan(&localDestination));
		startNode.fCost = startNode.hCost;
		startNode.flags = (startNode.flags & ~0xFF) | 1;
		goalNode.position = localDestination;

		const auto startPathGridNode = findClosestReachableNode(this, nodes, localStart);
		const auto destinationPathGridNode = findClosestReachableNode(this, nodes, localDestination);

		if (!startPathGridNode || !destinationPathGridNode) {
			return false;
		}

		std::vector<SearchNode> searchNodes;
		searchNodes.reserve(nodes.size());
		for (auto node : nodes) {
			searchNodes.push_back({ node, nullptr, INT_MAX, INT_MAX, false, false });
		}

		const auto startNode = findSearchNode(searchNodes, startPathGridNode);
		const auto destinationNode = findSearchNode(searchNodes, destinationPathGridNode);
		const auto destinationNodePosition = destinationPathGridNode->getLocalPosition();
		auto startNodePosition = startPathGridNode->getLocalPosition();
		startNode->gCost = 0;
		startNode->fCost = int(startNodePosition.distanceManhattan(&destinationNodePosition));
		startNode->opened = true;

		while (true) {
			SearchNode* current = nullptr;
			auto currentCost = INT_MAX;

			for (auto& node : searchNodes) {
				if (node.opened && !node.closed && node.fCost < currentCost) {
					currentCost = node.fCost;
					current = &node;
				}
			}

			if (!current) {
				return false;
			}

			if (current == destinationNode) {
				break;
			}

			current->closed = true;

			if (!current->pathGridNode->connectedNodes) {
				continue;
			}

			for (auto connectedNodePointer : *current->pathGridNode->connectedNodes) {
				if (!connectedNodePointer || !*connectedNodePointer) {
					continue;
				}

				auto connected = findSearchNode(searchNodes, *connectedNodePointer);
				if (!connected || connected->closed) {
					continue;
				}

				const auto connectedPosition = connected->pathGridNode->getLocalPosition();
				const auto tentativeGCost = current->gCost + 1;

				if (!connected->opened || tentativeGCost < connected->gCost) {
					connected->parent = current;
					connected->gCost = tentativeGCost;
					connected->fCost = tentativeGCost + int(connectedPosition.distanceManhattan(&destinationNodePosition));
					connected->opened = true;
				}
			}
		}

		std::vector<PathGrid::Node*> pathNodes;
		for (auto node = destinationNode; node; node = node->parent) {
			pathNodes.push_back(node->pathGridNode);
			if (node == startNode) {
				break;
			}
		}

		if (pathNodes.empty() || pathNodes.back() != startPathGridNode) {
			return false;
		}

		for (auto it = pathNodes.rbegin(); it != pathNodes.rend(); ++it) {
			if (*it == startPathGridNode) {
				continue;
			}

			pathList.push_back(toWorldPoint(this, (*it)->getLocalPosition()));
		}

		pathList.push_back(toWorldPoint(this, localDestination));
		return pathList.count != 0;
	}

	const auto TES3_PathPlanner_clearSearchStateAndPath = reinterpret_cast<void(__thiscall*)(PathPlanner*)>(0x544F10);
	void PathPlanner::clearSearchStateAndPath() {
		TES3_PathPlanner_clearSearchStateAndPath(this);
	}
}
