#include "Object.h"

Object::Object(ObjectManager& owner, std::string name)
    : m_Name(name)
    , m_Owner(owner)
    , m_NextCompID(2)
    , m_ToInitialize(0)
    , m_ToInitializeNextFrame(0)
    , m_ToDestroy(0) {
    IComponent* root = &m_Root;
    root->m_Object = this;
    root->m_ID = 1;
}

Object::Object(const Object& other, std::string name)
    : m_Name(name.empty() ? other.Name() + "_copy" : name)
    , m_Owner(other.m_Owner)
    , m_Root(other.m_Root)
    , m_NextCompID(other.m_NextCompID)
    , m_ToInitialize(other.m_ToInitialize)
    , m_ToInitializeNextFrame(other.m_ToInitializeNextFrame)
    , m_ToDestroy(other.m_ToDestroy) {

    // Copy root component
    IComponent* root = &m_Root;
    root->m_Object = this;

    // Copy components
    for (auto& comp : other.m_Components) {
        m_Components.emplace_back(comp->Clone());
        m_Components[m_Components.size() - 1]->m_Object = this;
        m_Components[m_Components.size() - 1]->m_ID = comp->m_ID;
        m_Components[m_Components.size() - 1]->Initialize();
    }

    // Copy connections between components
    // TODO
}

void Object::ProcessFrame() {
    InitializeComponents();
    UpdateComponents();
    DestroyComponents();
}

void Object::InitializeComponents() {
    m_ToInitialize = m_ToInitializeNextFrame;
    m_ToInitializeNextFrame = 0;
    for (; m_ToInitialize > 0; m_ToInitialize--) {
        m_Components[m_Components.size() - m_ToInitialize]->Initialize();
    }
}

void Object::UpdateComponents() {
    for (Components_t::size_type i = m_Components.size() - 1 - m_ToInitializeNextFrame; i > m_ToDestroy; i--) {
        m_Components[i]->Update();
    }
}

void Object::DestroyComponents() {
    if (m_ToDestroy > 0) {
        for (Components_t::size_type i = m_ToDestroy; i > 0; i--) {
            m_MessageManager.RemoveConnections(m_Components[i].get());
            m_Components[i]->Destroy();
        }
        m_Components.erase(m_Components.begin(), m_Components.begin() + m_ToDestroy);
        m_ToDestroy = 0;
    }
}

void Object::Disconnect(IMessageOut& sender, IMessageIn& receiver) {
    m_MessageManager.Disconnect(&sender, &receiver);
}

void Object::MarkToDestroy(Components_t::iterator it) {
    // Check if component hasn't been already marked
    if (std::distance(it, m_Components.begin()) > static_cast<ptrdiff_t>(m_ToDestroy)) {
        m_ToDestroy = m_ToDestroy + 1;
        std::iter_swap(m_Components.begin() + m_ToDestroy, it);
    }
}
