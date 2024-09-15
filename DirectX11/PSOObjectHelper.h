#pragma once
#include <list>
#include <vector>

#define ManageForRendering(Object)															\
protected:																					\
	std::list<Object*> ##Object##s;															\
public:																						\
	inline void Add##Object##(Object* ObjectIn) { ##Object##s.emplace_back(ObjectIn); }		\
	inline void Remove##Object##(Object* ObjectIn) { ##Object##s.remove(ObjectIn); }		


#ifdef _DEBUG
	#define CONSTANT_BUFFER_SETTER(Shader)																					\
	public:																													\
			inline void Set##Shader##ConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ConstantBuffers)	\
			{																												\
				memset(&Is##Shader##ConstBufferSet[StartSlot], true, NumBuffers);											\
				DeviceContextCached->##Shader##SetConstantBuffers(StartSlot, NumBuffers, ConstantBuffers);					\
			}

	#define SRV_SETTER(Shader)																									\
	public:																														\
			inline void Set##Shader##ShaderResourceViews(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* SRVs)	\
			{																													\
				memset(&Is##Shader##SRVSet[StartSlot], true, NumViews);															\
				DeviceContextCached->##Shader##SetShaderResources(StartSlot, NumViews, SRVs);									\
			}

	#define CONSTANT_BUFFER_RESETTER(Shader)																		\
	public:																											\
			inline void Reset##Shader##ConstantBuffers(UINT StartSlot, UINT NumBuffers)								\
			{																										\
				memset(&Is##Shader##ConstBufferSet[StartSlot], false, NumBuffers);									\
				std::vector<ID3D11Buffer*> ConstantBuffers;															\
				ConstantBuffers.resize(NumBuffers, nullptr);														\
				DeviceContextCached->##Shader##SetConstantBuffers(StartSlot, NumBuffers, ConstantBuffers.data());	\
			}

	#define SRV_RESETTER(Shader)																					\
	public:																											\
			inline void Reset##Shader##ShaderResourceViews(UINT StartSlot, UINT NumViews)							\
			{																										\
				memset(&Is##Shader##SRVSet[StartSlot], false, NumViews);											\
				std::vector<ID3D11ShaderResourceView*> SRVs;														\
				SRVs.resize(NumViews, nullptr);																		\
				DeviceContextCached->##Shader##SetShaderResources(StartSlot, NumViews, SRVs.data());				\
			}
#else
	#define CONSTANT_BUFFER_SETTER(Shader)																						\
		public:																													\
				inline void Set##Shader##ConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ConstantBuffers)	\
				{																												\
					DeviceContextCached->##Shader##SetConstantBuffers(StartSlot, NumBuffers, ConstantBuffers);					\
				}

	#define SRV_SETTER(Shader)																										\
		public:																														\
				inline void Set##Shader##ShaderResourceViews(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* SRVs)	\
				{																													\
					DeviceContextCached->##Shader##SetShaderResources(StartSlot, NumViews, SRVs);									\
				}

	#define CONSTANT_BUFFER_RESETTER(Shader)																			\
		public:																											\
				inline void Reset##Shader##ConstantBuffers(UINT StartSlot, UINT NumBuffers)								\
				{																										\
					std::vector<ID3D11Buffer*> ConstantBuffers;															\
					ConstantBuffers.resize(NumBuffers, nullptr);														\
					DeviceContextCached->##Shader##SetConstantBuffers(StartSlot, NumBuffers, ConstantBuffers.data());	\
				}

	#define SRV_RESETTER(Shader)																						\
		public:																											\
				inline void Reset##Shader##ShaderResourceViews(UINT StartSlot, UINT NumViews)							\
				{																										\
					std::vector<ID3D11ShaderResourceView*> SRVs;														\
					SRVs.resize(NumViews, nullptr);																		\
					DeviceContextCached->##Shader##SetShaderResources(StartSlot, NumViews, SRVs.data());				\
				}
#endif // DEBUG

