//
//  Area.cpp
//  Phantasma
//
//  Created by Thomas Harte on 25/12/2013.
//  Copyright (c) 2013 Thomas Harte. All rights reserved.
//

#include "freescape/area.h"
#include "freescape/objects/geometricobject.h"
#include "freescape/objects/entrance.h"
#include "common/algorithm.h"
#include "freescape/objects/object.h"

namespace Freescape {

Object *Area::objectWithIDFromMap(ObjectMap *map, uint16 objectID) {
	if (!map)
		return nullptr;
	if(!map->contains(objectID))
		return nullptr;
	return (*map)[objectID];
}

Object *Area::objectWithID(uint16 objectID) {
	return objectWithIDFromMap(objectsByID, objectID);
}

Object *Area::entranceWithID(uint16 objectID) {
	return objectWithIDFromMap(entrancesByID, objectID);
}

Object *Area::firstEntrance() {
	return entrancesByID->begin()->_value;
}

uint16 Area::getAreaID() {
	return areaID;
}

uint16 Area::getAreaFlags() {
	return areaFlags;
}

uint8 Area::getScale() {
	return scale;
}

Area::Area(
	uint16 _areaID,
	uint16 _areaFlags,
	ObjectMap *_objectsByID,
	ObjectMap *_entrancesByID,
	uint8 _scale,
	uint8 _skyColor,
	uint8 _groundColor,
	Graphics::PixelBuffer *_palette) {
	scale = _scale;
	palette = _palette;
	skyColor = _skyColor;
	groundColor = _groundColor;
	areaID = _areaID;
	areaFlags = _areaFlags;
	objectsByID = _objectsByID;
	entrancesByID = _entrancesByID;

	// create a list of drawable objects only
	for (ObjectMap::iterator iterator = objectsByID->begin(); iterator != objectsByID->end(); iterator++) {
		if (iterator->_value->isDrawable()) {
			drawableObjects.push_back(iterator->_value);
		}
	}

	// sort so that those that are planar are drawn last
	struct
	{
		bool operator()(Object *object1, Object *object2) {
			if (!object1->isPlanar() && object2->isPlanar())
				return false;
			if (object1->isPlanar() && !object2->isPlanar())
				return true;
			return object1->getObjectID() > object2->getObjectID();
		};
	} compareObjects;

	Common::sort(drawableObjects.begin(), drawableObjects.end(), compareObjects);
}

Area::~Area() {
	for (ObjectMap::iterator iterator = entrancesByID->begin(); iterator != entrancesByID->end(); iterator++)
		delete iterator->_value;

	for (ObjectMap::iterator iterator = objectsByID->begin(); iterator != objectsByID->end(); iterator++)
		delete iterator->_value;

	delete entrancesByID;
	delete objectsByID;
}

void Area::show() {
	for (ObjectMap::iterator it = objectsByID->begin(); it != objectsByID->end(); it++)
		debug("objID: %d, type: %d", (*it)._value->getObjectID(), (*it)._value->getType());

	for (ObjectMap::iterator it = entrancesByID->begin(); it != entrancesByID->end(); it++)
		debug("objID: %d, type: %d (entrance)", (*it)._value->getObjectID(), (*it)._value->getType());
}

void Area::loadObjectFlags(Common::SeekableReadStream *stream) {
	for (int i = 0; i < int(objectsByID->size()); i++) {
		uint16 key = stream->readUint32LE();
		assert(objectsByID->contains(key));
		Object *obj = (*objectsByID)[key];
		obj->setObjectFlags(stream->readUint32LE());
	}
}

void Area::saveObjectFlags(Common::WriteStream *stream) {
	int dirtyFlags = 0;
	//stream->writeUint32LE(objectsByID->size());

	for (ObjectMap::iterator iterator = objectsByID->begin(); iterator != objectsByID->end(); iterator++) {
		stream->writeUint32LE(iterator->_key);
		stream->writeUint32LE(iterator->_value->getObjectFlags());
	}
}

void Area::draw(Freescape::Renderer *gfx) {
	if (palette)
		gfx->_palette = palette;

	if (areaFlags & 0x80)
		gfx->_keyColor = 0;
	else
		gfx->_keyColor = 255;

	gfx->clear();
	if (areaFlags & 0x01)
		gfx->drawFloor(groundColor);

	if (areaFlags & 0x02)
		gfx->drawSky(skyColor);

	assert(drawableObjects.size() > 0);
	for (Common::Array<Object *>::iterator it = drawableObjects.begin(); it != drawableObjects.end(); it++) {
		if (!(*it)->isDestroyed() && !(*it)->isInvisible()) {
			(*it)->draw(gfx);
		}
	}
}

Object *Area::shootRay(const Math::Ray &ray) {
	float size = 16 * 8192; // TODO: check if this is max size
	Object *collided = nullptr;
	for (int i = 0; i < int(drawableObjects.size()); i++) {
		float objSize = drawableObjects[i]->getSize().length();
		if (!drawableObjects[i]->isDestroyed() && !drawableObjects[i]->isInvisible()
		  && drawableObjects[i]->_boundingBox.isValid()
		  && ray.intersectAABB(drawableObjects[i]->_boundingBox)
		  && size >= objSize) {
			debug("shot obj id: %d", drawableObjects[i]->getObjectID());
			collided = drawableObjects[i];
			size = objSize;
		}
	}
	return collided;
}

Object *Area::checkCollisions(const Math::AABB &boundingBox) {
	float size = 3 * 8192 * 8192; // TODO: check if this is max size
	Object *collided = nullptr;
	for (int i = 0; i < int(drawableObjects.size()); i++) {
		if (!drawableObjects[i]->isDestroyed() && !drawableObjects[i]->isInvisible()) {
			GeometricObject *obj = (GeometricObject*) drawableObjects[i];
			float objSize = obj->getSize().length();
			if (obj->collides(boundingBox) && size > objSize) {
				collided = obj;
				size = objSize;
			}
		}
	}
	return collided;
}

void Area::addStructure(Area *structure) {
	Object *obj = nullptr;
	if (!structure || !entrancesByID->contains(255)) {
		int id = 254;
		Common::Array<uint8> *gColors = new Common::Array<uint8>;
		for (int i = 0; i < 6; i++)
			gColors->push_back(0);

		obj = (Object*) new GeometricObject(
			Object::Type::Cube,
			id,
			0, // flags
			Math::Vector3d(0, -1, 0), // Position
			Math::Vector3d(4128, 1, 4128), // size
			gColors,
			nullptr,
			FCLInstructionVector()
		);
		(*objectsByID)[id] = obj;
		drawableObjects.insert_at(0, obj);
		return;
	}
	RoomStructure *rs = (RoomStructure*) (*entrancesByID)[255];

	for (int i = 0; i < int(rs->structure.size()); i++) {
		int16 id = rs->structure[i];
		if (id == 0)
			continue;

		debug("Adding object %d to room structure", id);
		obj = structure->objectWithID(id);
		if (!obj) {
			assert(structure->entranceWithID(id));
			(*entrancesByID)[id] = structure->entranceWithID(id);
		} else {
			(*objectsByID)[id] = structure->objectWithID(id);
			if (obj->isDrawable())
				drawableObjects.insert_at(0, obj);
		}
	}
}

} // End of namespace Freescape