#include "TES3PathPlanner.h"

#include "TES3Cell.h"
#include "TES3Util.h"

#include "MWSEConfig.h"

namespace TES3 {
	namespace {
		constexpr auto MaxAttachmentDistance = 1024.0f;

		struct SearchNode {
			PathGrid::Node* pathGridNode;
			SearchNode* parent;
			int gCost;
			int fCost;
			int component;
			bool closed;
			bool opened;
		};

		struct ReachableNodeCandidate {
			PathGrid::Node* pathGridNode;
			int score;
			int component;
		};

		struct PathSearchResult {
			std::vector<PathGrid::Node*> pathNodes;
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

		void resetSearchNodes(std::vector<SearchNode>& searchNodes) {
			for (auto& node : searchNodes) {
				node.parent = nullptr;
				node.gCost = INT_MAX;
				node.fCost = INT_MAX;
				node.closed = false;
				node.opened = false;
			}
		}

		void markConnectedComponents(std::vector<SearchNode>& searchNodes) {
			auto nextComponent = 0;
			for (auto& root : searchNodes) {
				if (root.component >= 0) {
					continue;
				}

				std::vector<SearchNode*> stack;
				stack.push_back(&root);
				root.component = nextComponent;

				while (!stack.empty()) {
					const auto current = stack.back();
					stack.pop_back();

					if (!current->pathGridNode->connectedNodes) {
						continue;
					}

					for (auto connectedNodePointer : *current->pathGridNode->connectedNodes) {
						if (!connectedNodePointer || !*connectedNodePointer) {
							continue;
						}

						auto connected = findSearchNode(searchNodes, *connectedNodePointer);
						if (connected && connected->component < 0) {
							connected->component = nextComponent;
							stack.push_back(connected);
						}
					}
				}

				++nextComponent;
			}
		}

		int getAttachmentScore(const NI::Point3& point, const NI::Point3& nodePosition) {
			return int(point.distanceManhattan(&nodePosition) + point.heightDifference(&nodePosition));
		}

		std::vector<ReachableNodeCandidate> findReachableNodeCandidates(
			const PathPlanner* planner,
			std::vector<SearchNode>& searchNodes,
			const std::vector<PathGrid::Node*>& nodes,
			const NI::Point3& point
		) {
			std::vector<ReachableNodeCandidate> candidates;
			PathGrid::Node* exactPathGridNode = nullptr;

			const auto exactIt = std::find_if(nodes.begin(), nodes.end(),
				[&point](const auto node) {
					return node->getLocalPosition() == point;
				}
			);
			if (exactIt != nodes.end()) {
				exactPathGridNode = *exactIt;
				if (const auto searchNode = findSearchNode(searchNodes, exactPathGridNode)) {
					candidates.push_back({ exactPathGridNode, 0, searchNode->component });
				}
			}

			candidates.reserve(nodes.size());
			auto worldPoint = toWorldPoint(planner, point);
			for (auto node : nodes) {
				if (node == exactPathGridNode) {
					continue;
				}

				auto nodePosition = node->getLocalPosition();
				if (point.distance(&nodePosition) > MaxAttachmentDistance) {
					continue;
				}

				auto worldNodePosition = toWorldPoint(planner, nodePosition);
				if (!mwse::tes3::testLineOfSight(&worldPoint, 64.0f, &worldNodePosition, 64.0f)) {
					continue;
				}

				if (const auto searchNode = findSearchNode(searchNodes, node)) {
					candidates.push_back({ node, getAttachmentScore(point, nodePosition), searchNode->component });
				}
			}

			std::sort(candidates.begin(), candidates.end(),
				[](const auto& lhs, const auto& rhs) {
					return lhs.score < rhs.score;
				}
			);

			candidates.erase(std::unique(candidates.begin(), candidates.end(),
				[](const auto& lhs, const auto& rhs) {
					return lhs.pathGridNode == rhs.pathGridNode;
				}
			), candidates.end());

			constexpr auto MaxCandidateCount = 8;
			if (candidates.size() > MaxCandidateCount) {
				candidates.resize(MaxCandidateCount);
			}

			return candidates;
		}

		std::optional<PathSearchResult> findPath(
			std::vector<SearchNode>& searchNodes,
			PathGrid::Node* startPathGridNode,
			PathGrid::Node* destinationPathGridNode
		) {
			resetSearchNodes(searchNodes);

			const auto startNode = findSearchNode(searchNodes, startPathGridNode);
			const auto destinationNode = findSearchNode(searchNodes, destinationPathGridNode);
			if (!startNode || !destinationNode) {
				return std::nullopt;
			}

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
					return std::nullopt;
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
				return std::nullopt;
			}

			return PathSearchResult{ pathNodes };
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

		std::vector<SearchNode> searchNodes;
		searchNodes.reserve(nodes.size());
		for (auto node : nodes) {
			searchNodes.push_back({ node, nullptr, INT_MAX, INT_MAX, -1, false, false });
		}
		markConnectedComponents(searchNodes);

		const auto startCandidates = findReachableNodeCandidates(this, searchNodes, nodes, localStart);
		const auto destinationCandidates = findReachableNodeCandidates(this, searchNodes, nodes, localDestination);
		if (startCandidates.empty() || destinationCandidates.empty()) {
			return false;
		}

		std::optional<PathSearchResult> selectedPath;
		for (const auto& destinationCandidate : destinationCandidates) {
			for (const auto& startCandidate : startCandidates) {
				if (startCandidate.component != destinationCandidate.component) {
					continue;
				}

				auto path = findPath(searchNodes, startCandidate.pathGridNode, destinationCandidate.pathGridNode);
				if (!path) {
					continue;
				}

				selectedPath = path;
				break;
			}

			if (selectedPath) {
				break;
			}
		}

		if (!selectedPath) {
			return false;
		}

		for (auto it = selectedPath->pathNodes.rbegin(); it != selectedPath->pathNodes.rend(); ++it) {
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
