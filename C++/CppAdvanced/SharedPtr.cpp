/*
  c++ -std=c++2b -Wall -Werror -Wextra -fsanitize=undefined,address -o SharedPtr SharedPtr.cpp
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

struct Person;

// ##########
// # People #
// ##########
struct People
{
  using PersonPtr = std::shared_ptr<Person>;
  std::vector<PersonPtr> world;
  std::vector<std::string> banned;

  PersonPtr finder(std::string name) const&;
  PersonPtr add(std::string name) &;
  bool      remove(std::string name) &;
  bool      ban(std::string name) &;
};

// ##########
// # Person #
// ##########
struct Person: std::enable_shared_from_this<Person>
{
  using FollowerPtr = std::shared_ptr<Person>;
  using FollowPtr   = std::weak_ptr<Person>;

  std::string              name{};
  std::vector<FollowerPtr> followers;
  std::vector<FollowPtr>   followings;
  std::vector<std::string> blocked;

  Person(std::string n):name{n}{}

  bool addFollower(auto& pe, std::string name) &;
  bool addFollowing(auto& pe, std::string name) &;
  void listFollowers(std::ostream& out) const&;
  void listFollowings(std::ostream& out) const&;
  bool block(std::string name) &;
};

template<typename T>
auto search(T& container, std::string name)
{
  if constexpr (std::is_same_v<T, std::vector<Person::FollowPtr>> == true)
      return std::find_if(container.begin(), container.end(), [&](auto c){ return c.lock()->name == name; });
  else
      return std::find_if(container.begin(), container.end(), [&](auto c){ return c->name == name; });
}

// ##########
// # People #
// ##########
People::PersonPtr People::finder(std::string name) const&
{
  auto it = search(People::world, name);
  if(it == People::world.end()) return nullptr;
  return *it;
}

People::PersonPtr People::add(std::string name) &
{
  if (std::find(People::banned.begin(), People::banned.end(), name) != People::banned.end()) return nullptr;
  auto it = People::finder(name);
  if (it != nullptr) return it;
  People::world.push_back(std::make_shared<Person>(name));
  return People::world.back();
}

bool People::remove(std::string name) &
{
  auto it = search(People::world, name);
  if (it == People::world.end()) return false;
  for(auto& person: People::world)
    {
      auto itIngs = search(person->followings, name);
      if (itIngs != person->followings.end())
        person->followings.erase(itIngs);
      else
        {
          auto itBlock = std::find(person->blocked.begin(), person->blocked.end(), name);
          if (itBlock != person->blocked.end())
            person->blocked.erase(itBlock);
        }

      auto itErs = search(person->followers, name);
      if (itErs != person->followers.end())
        person->followers.erase(itErs);
    }
  People::world.erase(it);
  return true;
}

bool People::ban(std::string name) &
{
  if (People::finder(name) == nullptr) return false;
  People::remove(name);
  People::banned.push_back(name);
  return true;
}

// ##########
// # Person #
// ##########
bool Person::addFollower(auto& pe, std::string name) &
{
  if((search(Person::followers, name) == Person::followers.end()) &&
     (std::find(Person::blocked.begin(), Person::blocked.end(), name) == Person::blocked.end()) &&
     (std::find(pe.banned.begin(), pe.banned.end(), name) == pe.banned.end()))
    {
      auto it = pe.finder(name);
      if(it == nullptr)
        Person::followers.push_back(pe.add(name));
      else
        Person::followers.push_back(it);
      Person::followers.back()->addFollowing(pe, this->name);
      return true;
    }
  return false;
}

bool Person::addFollowing(auto& pe, std::string name) &
{
  if((search(Person::followings, name) == Person::followings.end()) &&
     (std::find(Person::blocked.begin(), Person::blocked.end(), name) == Person::blocked.end()) &&
     (std::find(pe.banned.begin(), pe.banned.end(), name) == pe.banned.end()))
    {
      auto it = pe.finder(name);
      if(it == nullptr)
        Person::followings.push_back(pe.add(name)->weak_from_this());
      else
        Person::followings.push_back(it->weak_from_this());
      Person::followings.back().lock()->addFollower(pe, this->name);
      return true;
    }
  return false;
}

bool Person::block(std::string name) &
{
  auto it = search(Person::followers, name);
  if(it != Person::followers.end())
    {
      auto toBlock = *it;
      Person::blocked.push_back(name);
      Person::followers.erase(it);
      auto me = search(toBlock->followings, this->name);
      toBlock->followings.erase(me);
      toBlock->blocked.push_back(this->name);
      return true;
    }
  return false;
}

void Person::listFollowers(std::ostream& out) const&
{
  out << "List of followers of " << Person::name << '\n';
  for(auto& p: Person::followers)
    out << "\t" << p->name << '\n';
}

void Person::listFollowings(std::ostream& out) const&
{
  out << "List of followings for " << Person::name << '\n';
  for(auto& p: Person::followings)
    out << "\t" << p.lock()->name << '\n';
}


int main()
{
  People pe;
  pe.add("Peter");
  pe.add("Amy");
  pe.add("Tom");

  auto p = pe.finder("Peter");
  p->addFollowing(pe, "Fred");
  p->addFollowing(pe, "Steve");
  p->addFollowing(pe, "Bibi");

  p = pe.finder("Fred");
  p->addFollowing(pe, "Peter");
  p->addFollowing(pe, "Steve");
  p->addFollowing(pe, "Amy");

  p = pe.finder("Steve");
  p->addFollowing(pe, "Fred");
  p->addFollowing(pe, "Peter");

  p = pe.finder("Amy");
  p->addFollowing(pe, "Fred");
  p->addFollowing(pe, "Bibi");

  p = pe.finder("Tom");
  p->addFollowing(pe, "Bibi");

  for(auto& p: pe.world)
    {
      p->listFollowers(std::cout);
      p->listFollowings(std::cout);
      std::cout << '\n';
    }

  // ############
  // # Blocking #
  // ############
  std::cout << "\n=== Blocking ===" << '\n';
  p = pe.finder("Bibi");
  p->block("Peter");
  p = pe.finder("Peter");
  if (p->addFollowing(pe, "Bibi") == true)
    std::cout << "Peter can follow Bibi" << '\n';
  else
    std::cout << "Peter can not follow Bibi" << '\n';

  // ###########
  // # Banning #
  // ###########
  std::cout << "\n=== Banning ===" << '\n';
  pe.ban("Bibi");
  p = pe.finder("Peter");
  p->addFollowing(pe, "Bibi");
  for(auto& p: pe.world)
    {
      p->listFollowers(std::cout);
      p->listFollowings(std::cout);
      std::cout << '\n';
    }

  return 0;
}
