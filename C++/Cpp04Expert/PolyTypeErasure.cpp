/*
  c++ -std=c++2b -Wall -Werror -Wextra -fsanitize=undefined,address -o PolyTypeErasure PolyTypeErasure.cpp
*/

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>


// #############
// # Drawables #
// #############
void draw(std::string s, std::ostream& out)
{
  out << "String (" << s << ")";
}

void draw(int i, std::ostream& out)
{
  out << "Int (" << i << ")";
}

struct rectangle
{
  rectangle(double w, double h) : width{w}, height{h} {}
  double width;
  double height;
};

void draw(rectangle const& r, std::ostream& out)
{
  out << "Rectangle (" << r.width << "," << r.height << ")";
}

struct circle
{
  circle(double r) : radius{r} {}

  void draw(std::ostream& out) const &
  {
    out << "Circle (" << radius << ")";
  }

private:
  double radius;
};


// ###############################
// # Polymprphic drawable widget #
// ###############################

template<typename T, typename U>
concept has_draw = requires (T t, U u) { t.draw(u); };

struct widget
{
  template<typename T>
  explicit widget(T x) : self_(std::make_unique<model<T>>(std::move(x))) {}
  // explicit means that the constructor cannot be used for implicit conversions and copy-initialization

  ~widget() = default; // Rule of 5 for Generale Manager class type
  // Non-virtual because there are no virtual member functions
  // in widget and it is not intended to be inherited

  // ###################################
  // # Copy constructor and assignment #
  // ###################################
  widget(widget const& x) noexcept : self_(x.self_->copy_()) {}
  widget& operator=(widget const& x) & noexcept
  {
    return *this = widget(x);
  }

  // ###################################
  // # Move constructor and assignment #
  // ###################################
  // From the Howard Hinnant table defining the destructor implies not declaring the move
  // operations, that's why if we want them we need to resurrect them with default
  widget(widget&&) noexcept = default;
  widget& operator=(widget&&) & noexcept = default;

  friend void draw(widget const& x, std::ostream& out)
  {
    x.self_->draw_(out);
  }

private:
  struct concept_t // Polymorphic base
  {
    virtual ~concept_t() = default;
    virtual std::unique_ptr<concept_t> copy_() const & = 0;
    virtual void draw_(std::ostream&) const & = 0;
    concept_t& operator=(concept_t&&) = delete; // Rule Destructor defined Deleted Move Assignment (DesDeMovA): non copiable, non movable
  };

  template<typename T>
  struct model: concept_t
  {
    model(T x) : data_(std::move(x)) {}

    std::unique_ptr<concept_t> copy_() const &
    {
      return std::make_unique<model>(this->data_); // Cloning
    }

    void draw_(std::ostream& out) const &
    {
      if constexpr (has_draw<T, std::ostream>)
        data_.draw(out); // Dispatch to memeber function
      else
        draw(data_, out); // Dispatch to free function
    }

    T data_;
  };

  std::unique_ptr<concept_t> self_;
};

struct composite
{
  template<typename T>
  void add(T w)
  {
    content.emplace_back(std::move(w));
  }

  friend void draw(composite const& c, std::ostream& out)
  {
    out << "{ ";
    for(widget const& drawable: c.content)
      {
        draw(drawable, out); out << ", ";
      }
    out << " }";
  }

private:
  std::vector<widget> content{};
};


// ##################
// # Test composite #
// ##################
void testComposite()
{
  std::ostringstream out{};

  composite c{};
  c.add(circle(double{42}));
  c.add(rectangle(double{4}, double{2}));
  c.add(circle(double{4}));
  c.add(42);
  c.add("A C string");
  widget w{c};
  draw(w, out);

  std::cout << out.str() << std::endl;
}


// #########################
// # Test dynamic dispatch #
// #########################
void useWidget(const widget& w)
{
  std::ostringstream out{};

  // The dynamic dispatch happens via the std::unique_ptr<concept_t>
  // where the corresponding virtual member function(s) are then
  // dispatched to the corresponding member functions of model<T>
  // which then calls the appropriate free function draw(...)
  draw(w, out);

  std::cout << out.str() << std::endl;
}

void testDynamicDispatch()
{
  composite c{};
  char choice;

  std::cout << "Choose shape (c or r): ";
  std::cin >> choice;

  // Implicit conversion of std::unique_ptr<model<circle>>
  // to std::uniuqe_ptr<concept_t>. The heap-allocated objects
  // remain of type model<circle> and model<rectangle>, respectively
  if (choice == 'c')
    c.add(circle(double{42}));
  else if (choice == 'r')
    c.add(rectangle(double{4}, double{2}));
  else
    {
      std::cout << "Wrong choice: " << choice << std::endl;
      return;
    }
  widget w{c};

  useWidget(w);
}


// ########################################################################
// # Explanation:                                                         #
// # The dynamic dispatch is encapsulated in the widget::model class      #
// # Whenever you create a widget with a new type T, that type will       #
// # instantiate a new version of model<T> providing the dynamic dispatch #
// # This instantiation happens at compile time on usage                  #
// # The dynamic dispatch through widget::concept_t's virtual functions   #
// # happens at run-time                                                  #
// ########################################################################


// ########
// # Main #
// ########
int main()
{
  testComposite();
  testDynamicDispatch();

  return 0;
}
