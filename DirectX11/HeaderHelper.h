#pragma once
#include <xutility>

#define MakeGetter(Variable)																		\
public:																								\
	inline const decltype(Variable)& Get##Variable() const { return Variable; };							

#define MakePointerGetter(Variable)																	\
public:																								\
	inline decltype(Variable)* GetPointer##Variable() { return &Variable; };							

#define MakeSmartPtrGetter(Variable)																\
public:																								\
	inline const auto Get##Variable() { return Variable.get(); };				

#define MakeComPtrGetter(Variable)																	\
public:																								\
	inline const auto Get##Variable() { return Variable.Get(); };				


#define MakeSetter(Variable)																		\
public:																								\
	inline void Set##Variable(const decltype(Variable)& ValueIn) { Variable = ValueIn; };			\
	inline void Set##Variable(decltype(Variable) && ValueIn) { Variable = std::move(ValueIn); };	


#define MakeSetterGetter(Variable)																	\
	MakeGetter(Variable)																			\
	MakeSetter(Variable)																			

#define MakeSmartPtrSetterGetter(Variable)															\
	MakeSmartPtrGetter(Variable)																			\
	MakeSetter(Variable)										