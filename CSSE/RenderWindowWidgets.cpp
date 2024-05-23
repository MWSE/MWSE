#include "RenderWindowWidgets.h"
#include "DialogRenderWindow.h"

#include "CSDataHandler.h"
#include "CSRecordHandler.h"
#include "CSModelLoader.h"

#include "NICamera.h"
#include "NIMatrix33.h"
#include "NILines.h"
#include "NILinesData.h"

#include "LogUtil.h"
#include "MathUtil.h"

namespace se::cs::dialog::render_window {
	WidgetsController::WidgetsController() {
		auto loader = DataHandler::get()->recordHandler->modelLoader;

		root = loader->loadNIF("meshes\\mwse\\widgets.nif");
		if (!root) {
			return;
		}
		{
			auto object = root->getObjectByNameAndType("axisLines", NI::RTTIStaticPtr::NiSwitchNode);
			if (object) {
				axisLines = static_cast<NI::SwitchNode*>(object);
				axisLines->setAppCulled(true);
			}
		}
		{
			auto object = root->getObjectByNameAndType("gridRoot", NI::RTTIStaticPtr::NiNode);
			if (object) {
				gridRoot = static_cast<NI::Node*>(object);
				gridRoot->setAppCulled(true);
			}
		}
		{
			auto object = root->getObjectByNameAndType("unitPlane", NI::RTTIStaticPtr::NiGeometry);
			if (object) {
				debugUnitPlane = static_cast<NI::Geometry*>(object);
				debugUnitPlane->setAppCulled(true);
			}
		}
		{
			auto object = root->getObjectByNameAndType("unitSphere", NI::RTTIStaticPtr::NiGeometry);
			if (object) {
				debugUnitSphere = static_cast<NI::Geometry*>(object);
				debugUnitSphere->setAppCulled(true);
			}
		}
		{
			auto object = root->getObjectByNameAndType("unitArrows", NI::RTTIStaticPtr::NiGeometry);
			if (object) {
				debugUnitArrows = static_cast<NI::Geometry*>(object);
				debugUnitArrows->setAppCulled(true);
			}
		}
		root->update();
		root->updateEffects();
		root->updateProperties();
	}

	WidgetsController::~WidgetsController() {
		if (root) {
			root->detachFromParent();
			DataHandler::get()->recordHandler->modelLoader->releaseNIF(root);
		}
	}

	void WidgetsController::show() {
		if (axisLines) {
			axisLines->setAppCulled(false);
		}
	}

	void WidgetsController::hide() {
		if (axisLines) {
			axisLines->setAppCulled(true);
		}
	}

	bool WidgetsController::isShown() {
		return axisLines && !axisLines->getAppCulled();
	}

	void WidgetsController::setPosition(NI::Vector3& position) {
		if (axisLines) {
			axisLines->localTranslate = position;
			axisLines->update();
		}
		if (gridRoot) {
			gridRoot->localTranslate = position;
			gridRoot->update();
		}
	}

	void WidgetsController::setAxis(WidgetsAxis axis) {
		if (axisLines) {
			axisLines->setSwitchIndex((int)axis);
		}
	}

	void WidgetsController::updateGridGeometry(float radius, int gridSnap) {
		if (!gridRoot) {
			return;
		}
		// Ensure non-zero grid size.
		float gridSize = std::max(float(gridSnap), 1.0f);

		// Ensure radius is big enough visually to see our grid.
		float gridRadius = std::max(radius, gridSize * 4.0f);

		// Calculate how many vertices we need to fill the grid.
		int g = 2.0f * (gridRadius / gridSize);

		// Needs an even number of vertices for NiLines to work.
		g += (g % 2) != 0;

		unsigned int vertexCount = g + 2;
		if (vertexCount > 32767) {
			return;
		}

		// Calculate unitized X coordinates for vertices.
		// Based on Bresenham's Circle Drawing Algorithm.
		std::vector<int> grid(vertexCount, 0);
		{
			int x = 0;
			int y = g;
			int d = 1 - g;
			while (x < y) {
				grid[y] = std::max(grid[y], g + x);
				grid[x] = std::max(grid[x], g + y);
				if (d <= 0) {
					d += (x << 1) + 3;
				}
				else {
					d += ((x - y) << 1) + 5;
					y -= 1;
				}
				x += 1;
			}
		}

		auto lines = NI::Lines::create(vertexCount, true, false);
		auto linesData = lines->getModelData();

		auto vertices = linesData->vertex;
		auto colors = linesData->color;
		auto flags = linesData->lineSegmentFlags;

		for (auto i = 0u; i < vertexCount; ++i) {
			auto& v = vertices[i];
			if (i % 2 == 0) {
				v.x = grid[i] * 0.5;
				v.y = i;
				v.z = 0;
			}
			else {
				v.x = -vertices[i - 1].x;
				v.y = vertices[i - 1].y;
				v.z = 0;
			}
			colors[i] = { 1.0f, 1.0f, 1.0f, 0.2f };
			flags[i] = (i % 2) == 0;
		}

		linesData->updateModelBound();

		for (auto& child : gridRoot->children) {
			if (child->isInstanceOfType(NI::RTTIStaticPtr::NiLines)) {
				auto node = static_cast<NI::Lines*>(child.get());
				node->setModelData(linesData);
				node->setAppCulled(false);
			}
		}
	}

	void WidgetsController::updateGridPosition(NI::Vector3 position, bool snapX, bool snapY, bool snapZ, int gridSnap) {
		if (!gridRoot) {
			return;
		}
		// Set position
		// If not snapping we default to XY axis.
		auto snapXY = !snapX && !snapY && !snapZ;
		if (snapX || snapXY) {
			position.x = std::roundf(position.x / gridSnap) * gridSnap;
		}
		if (snapY || snapXY) {
			position.y = std::roundf(position.y / gridSnap) * gridSnap;
		}
		if (snapZ) {
			position.z = std::roundf(position.z / gridSnap) * gridSnap;
		}
		gridRoot->localTranslate = position;

		// Set rotation
		// If we're moving on Z axis, align the grid vertically.
		if (snapZ) {
			const auto worldUp = NI::Vector3(0, 0, 1);
			auto camera = RenderController::get()->camera;

			auto up = position - camera->worldTransform.translation;
			up.z = 0.0;
			up.normalize();

			auto left = worldUp.crossProduct(&up);
			left.normalize();

			auto forward = up.crossProduct(&left);
			forward.normalize();

			auto rotation = NI::Matrix33(
				-left.x, forward.x, up.x,
				-left.y, forward.y, up.y,
				-left.z, forward.z, up.z
			);
			rotation.reorthogonalize();

			gridRoot->setLocalRotationMatrix(&rotation);
		}
		// Otherwise we're on XY axes, clear any vertical alignment.
		else {
			gridRoot->getLocalRotationMatrix()->toIdentity();
		}

		// Set Scale
		float f = std::max(float(gridSnap), 1.0f);
		gridRoot->localScale = f / 2.0f;
	}

	void WidgetsController::showGrid() {
		if (gridRoot) {
			gridRoot->setAppCulled(false);
			gridRoot->update();
			gridRoot->updateEffects();
			gridRoot->updateProperties();
		}
	}

	void WidgetsController::hideGrid() {
		if (gridRoot) {
			gridRoot->setAppCulled(true);
		}
	}

	bool WidgetsController::isGridShown() {
		return gridRoot && !gridRoot->getAppCulled();
	}

	void WidgetsController::updateAngleGuideGeometry(float radius, float angleSnap) {
		if (!gridRoot) {
			return;
		}

		float angleRadians = math::degreesToRadians(angleSnap);

		unsigned int vertexCount = int(math::M_2PIf / angleRadians) * 2;
		if (vertexCount > 32767) {
			return;
		}

		auto innerRadius = radius * 1.2f;
		auto outerRadius = radius * 1.4f;

		auto lines = NI::Lines::create(vertexCount, true, false);
		auto linesData = lines->getModelData();

		auto vertices = linesData->vertex;
		auto colors = linesData->color;
		auto flags = linesData->lineSegmentFlags;

		for (auto i = 0u; i < vertexCount; i += 2) {
			auto angle = angleRadians * float(i / 2);
			float cs = cos(angle);
			float sn = sin(angle);

			vertices[i]     = { innerRadius * cs, innerRadius * sn, 0.0 };
			vertices[i + 1] = { outerRadius * cs, outerRadius * sn, 0.0 };

			colors[i]     = { 1.0f, 1.0f, 1.0f, 0.5f };
			colors[i + 1] = { 1.0f, 1.0f, 1.0f, 0.5f };

			flags[i]     = 1;
			flags[i + 1] = 0;
		}

		linesData->updateModelBound();

		for (auto& child : gridRoot->children) {
			child->setAppCulled(true);
		}

		auto& child = gridRoot->children[0];
		if (child->isInstanceOfType(NI::RTTIStaticPtr::NiLines)) {
			auto node = static_cast<NI::Lines*>(child.get());
			node->setModelData(linesData);
			node->setAppCulled(false);
		}
	}

	void WidgetsController::updateAngleGuidePosition(NI::Vector3 position, bool snapX, bool snapY, bool snapZ, int gridSnap) {
		if (!gridRoot) {
			return;
		}

		auto direction = NI::Vector3(0.0, 0.0, 1.0);
		if (snapX) {
			direction.x = 1.0f;
		}
		else if (snapY) {
			direction.y = 1.0f;
		}
		else if (snapZ) {
			direction.z = 1.0f;
		}

		NI::Matrix33 rotation;
		rotation.toRotationDifference(NI::Vector3(0, 0, 1), direction);
		gridRoot->setLocalRotationMatrix(&rotation);
		gridRoot->localTranslate = position;
		gridRoot->localScale = 1.0;
	}
}
