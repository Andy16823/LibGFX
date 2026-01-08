#pragma once
namespace LibGFX {
	struct QueueFamilyIndices {
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool isValid() {
			return graphicsFamily >= 0 && presentFamily >= 0;
		}

		bool gpShared() {
			return graphicsFamily == presentFamily;
		}
	};
}