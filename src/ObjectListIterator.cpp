#include "ObjectListIterator.h"

namespace df {
ObjectListIterator::ObjectListIterator(const ObjectList* p_l) {
  m_current_index = 0;
  m_p_list = p_l;
}

void ObjectListIterator::first() { m_current_index = 0; }

void ObjectListIterator::next() {
  m_current_index = std::min(m_current_index + 1, m_p_list->m_count);
}

bool ObjectListIterator::isDone() const {
  return m_current_index >= m_p_list->m_count;
}

Object* ObjectListIterator::currentObject() const {
  if (m_current_index < m_p_list->m_count) {
    return m_p_list->m_p_obj[m_current_index];
  }
  return nullptr;
}

}  // namespace df
