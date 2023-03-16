#pragma once

#include "entity/behavior.hpp"
#include "scene/iscene.hpp"
#include "scene/components/components.hpp"

using namespace TWE;

class CollisionDetectionTest : public Behavior {
private:
	MeshRendererComponent* mrc;
	AudioComponent* ac;
public:
	void start() override {
		ImGui::SetCurrentContext(imguiContext);
		ImGui::GetIO().IniFilename = nullptr;
	}

	void update(float deltaTime) override {
		ImGui::Begin("asddas", nullptr);
		ImGui::End();
	}

	void collisionDetection(Entity collidedEntity, const btCollisionObject* collisionObj) override {
		mrc = getComponent<MeshRendererComponent>();
		if(mrc->getMaterial().objColor != glm::vec3{1.f, 0.f, 0.f}) {
			if(!hasComponent<AudioComponent>())
				addComponent<AudioComponent>(gameObject.getScene()->getSceneAudio()->getSoundEngine());
			ac = getComponent<AudioComponent>();
			auto soundSource = ac->addSoundSource(rootPath.string() + '\\' + "audio\\vot-blt-ti-nh.mp3", true, 0.1f);
			soundSource->play();
		}
		mrc->getMaterial().objColor = {1.f, 0.f, 0.f};
	}
};

extern "C" __declspec(dllexport) Behavior* createCollisionDetectionTest();