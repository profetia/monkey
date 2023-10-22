#ifndef MONKEY_OBJECT_ENV_H_
#define MONKEY_OBJECT_ENV_H_

#include <memory>
#include <string>
#include <unordered_map>

namespace monkey::object {

class Object;

class Env {
 public:
  Env();
  explicit Env(std::shared_ptr<Env> outer);

  void set(const std::string &name, std::shared_ptr<Object> value);
  std::shared_ptr<Object> get(const std::string &name) const;

 private:
  std::unordered_map<std::string, std::shared_ptr<Object>> store_;
  std::shared_ptr<Env> outer_;
};

}  // namespace monkey::object

#endif  // MONKEY_OBJECT_ENV_H_