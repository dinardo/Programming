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
void draw(std::string s, std::ostream& os)
{
  os << "String: " << s;
}

void draw(int i, std::ostream& os)
{
  os << "Int: " << i;
}

struct rect
{
  rect(double w, double h) : width{w}, height{h} {}
  double width;
  double height;
};

void draw(rect const& r, std::ostream& on)
{
  on << "Rectangle: " << r.width << "," << r.height;
}

struct circle
{
  circle(double r) : radius{r} {}
  double radius;
};

void draw(circle const& c, std::ostream& on)
{
  on << "Circle: " << c.radius;
}


// ###############################
// # Polymprphic drawable widget #
// ###############################
struct widget
{
  template<typename T>
  widget(T x) : self_(std::make_unique<model<T>>(std::move(x))) {} // Mauro: need to make it explicit?

  ~widget() = default; // Rule of 5, non-virtual

  widget(widget const& x) : self_(x.self_->copy_()) {}

  widget(widget&&) noexcept = default; // Mauro: DesDeMovA?

  widget& operator=(widget const& x) &
  {
    return *this = widget(x);
  }

  widget& operator=(widget&&) & noexcept = default;

  friend void draw(widget const& x, std::ostream& out)
  {
    x.self_->draw_(out);
  }

private:
  struct concept_t // Polymorphic base
  {
    virtual ~concept_t() = default;
    virtual std::unique_ptr<concept_t> copy_() const = 0;
    virtual void draw_(std::ostream&) const = 0;
    concept_t& operator=(concept_t&&) = delete;
  };

  template<typename T>
  struct model: concept_t
  {
    model(T x) : data_(std::move(x)) {}

    std::unique_ptr<concept_t> copy_() const
    {
      return std::make_unique<model>(this->data_); // Cloning
    }

    void draw_(std::ostream& out) const
    {
      draw(data_, out); // Dispatch to global function
    }

    T data_;
  };

  std::unique_ptr<concept_t> self_;
};

using widgets = std::vector<widget>;

struct composite
{
  void add(widget w)
  {
    content.emplace_back(std::move(w));
  }

  friend void draw(composite const& c, std::ostream& on)
  {
    on << "{ ";
    for(widget const& drawable: c.content)
      {
        draw(drawable, on); on << ", ";
      }
    on << " }";
  }

private:
  widgets content{};
};


// ###############
// # Make a test #
// ###############
void testComposite()
{
  std::ostringstream out{};

  composite c{};
  c.add(circle(double{42}));
  c.add(rect(double{4}, double{2}));
  c.add(circle(double{4}));
  c.add(42);
  c.add("A C string");
  widget w{c};
  draw(w, out);

  std::cout << out.str() << std::endl;
}


// ########
// # Main #
// ########
int main()
{
  testComposite();

  return 0;
}
