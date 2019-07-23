#include "ObjectManager.h"

#include "Object.h"
#include "../scenes/IScene.h"

ObjectManager::ObjectManager(IScene& owner)
    : m_Scene(owner) {
    
}

ObjectManager::~ObjectManager() {
    for (auto it = m_Objects.begin(); it != m_Objects.end(); ++it) {
        delete (*it);
    }
}

void ObjectManager::ProcessFrame() {
    for (auto it = m_Objects.begin(); it != m_Objects.end(); it++) {
        (*it)->ProcessFrame();
    }
}

void ObjectManager::InitializeObjects() {
    for (auto it = m_Objects.begin(); it != m_Objects.end(); ++it) {
        (*it)->InitializeComponents();
    }
}

void ObjectManager::UpdateObjects() {
    for (auto it = m_Objects.begin(); it != m_Objects.end(); ++it) {
        (*it)->UpdateComponents();
    }
}

void ObjectManager::DestroyObjects() {
    for (auto it = m_Objects.begin(); it != m_Objects.end(); ++it) {
        (*it)->DestroyComponents();
    }
}

Object* ObjectManager::CreateObject(std::string name) {
    m_Objects.push_back(new Object(*this, name));
    
    return m_Objects.at(m_Objects.size() - 1);
}

Object* ObjectManager::CreateObject(const Object* other, std::string name) {
    m_Objects.push_back(new Object(*other, name));
    
    return m_Objects.at(m_Objects.size() - 1);
}
