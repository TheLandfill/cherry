#pragma once

#define COMMA ,

#ifdef USE_INTERFACES
#define COMMA_IN_CLASS_INHERITANCE ,
#define ABELIAN_GROUP(T) : public Abelian_Group<T>
#define RING(T) : public Ring<T>
#define DIVISION_RING(T) : public Division_Ring<T>
#define OVERRIDE override
#else
#define COMMA_IN_CLASS_INHERITANCE
#define ABELIAN_GROUP(T)
#define RING(T)
#define DIVISION_RING(T)
#define OVERRIDE
#endif
