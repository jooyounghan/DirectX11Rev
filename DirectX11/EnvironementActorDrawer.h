#pragma once
#include "IInformationDrawer.h"

#include <memory>
#include <unordered_map>
#include <string>

class EnvironmentActor;
class AssetManager;

class EnvironementActorDrawer : AInformationDrawer<EnvironmentActor>
{
public: EnvironementActorDrawer(EnvironmentActor* EnvironmentActorIn);

public:
	virtual void DrawInformation() override;

private:
	void DrawBackgroundEXRTexture();
	void DrawIBLTextures();
	void DrawToneMappingConstant();

private:
	template<typename T>
	std::shared_ptr<T> DrawAndSelectNormalTexture(
		const std::unordered_map<std::string, std::shared_ptr<T>>& ManagingTexturesIn,
		T* CurrentSelected, const char* StrId, const char* PreviewText
	);
};
