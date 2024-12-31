#pragma once

class ZIndexable;

class ZIndexComparer
{
public:
    bool operator()(ZIndexable* zIndex1, ZIndexable* zIndex2) const;
};
