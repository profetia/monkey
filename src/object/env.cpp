#include <monkey/object/env.h>
#include <monkey/object/object.h>

#include <memory>
#include <string>
#include <utility>

namespace monkey::object {

Env::Env(std::shared_ptr<Env> outer) : outer_(std::move(outer)) {}

void Env::set(const std::string &name, std::unique_ptr<Object> value) {
  store_[name] = std::move(value);
}

std::unique_ptr<Object> Env::get(const std::string &name) {
  auto it = store_.find(name);
  if (it == store_.end()) {
    return nullptr;
  }
  auto object = std::move(it->second);
  store_.erase(it);
  return object;
}

bool Env::contains(const std::string &name) const {
  return store_.find(name) != store_.end();
}

}  // namespace monkey::object