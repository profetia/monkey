#include <monkey/eval/builtin.h>
#include <monkey/object/env.h>
#include <monkey/object/object.h>

#include <memory>
#include <string>
#include <utility>

namespace monkey::object {

Env::Env() : outer_(nullptr) {
  set("len", std::make_shared<Builtin>(eval::builtin::len));
  set("first", std::make_shared<Builtin>(eval::builtin::first));
  set("last", std::make_shared<Builtin>(eval::builtin::last));
  set("rest", std::make_shared<Builtin>(eval::builtin::rest));
  set("push", std::make_shared<Builtin>(eval::builtin::push));
  set("puts", std::make_shared<Builtin>(eval::builtin::puts));
}

Env::Env(std::shared_ptr<Env> outer) : outer_(std::move(outer)) {}

void Env::set(const std::string &name, std::shared_ptr<Object> value) {
  store_[name] = std::move(value);
}

std::shared_ptr<Object> Env::get(const std::string &name) const {
  auto it = store_.find(name);
  if (it != store_.end()) {
    return it->second;
  }
  if (outer_ != nullptr) {
    return outer_->get(name);
  }
  return nullptr;
}

}  // namespace monkey::object