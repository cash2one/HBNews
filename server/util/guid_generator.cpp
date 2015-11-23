#include "guid_generator.h"

GuidGenerator& GetGuidGenerator(){
	static GuidGenerator instance;
	return instance;
}