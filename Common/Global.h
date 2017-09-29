#pragma once

#ifdef BASE_LIB_EXPORT_API
# define BASE_LIB_EXPORT __declspec(dllexport)
#else
# define BASE_LIB_EXPORT __declspec(dllimport)
#endif