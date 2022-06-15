#ifndef OPTIONAL_H_
#define OPTIONAL_H_

template <typename T>
class Optional
{
private:
  T *data;

  void free()
  {
    delete this->data;
    this->data = nullptr;
  }

  void setData(const T &data)
  {
    if (this->data != &data)
    {
      this->free();
      this->data = new T(data);
    }
  }

public:
  Optional() : data(nullptr) {}

  Optional(const T &data) : data(nullptr)
  {
    this->setData(data);
  }

  Optional(const Optional &other) : data(nullptr)
  {
    if (other.isNull())
    {
      this->free();
    }
    else
    {
      this->setData(other.getData());
    }
  }

  Optional(Optional &&other)
  {
    data = other.data;
    other.data = nullptr;
  }

  Optional &operator=(const Optional &other)
  {
    if (this != &other)
    {
      if (other.isNull())
      {
        this->free();
      }
      else
      {
        this->setData(other.getData());
      }
    }

    return *this;
  }

  Optional &operator=(Optional &&other)
  {
    if (this != &other)
    {
      data = other.data;
      other.data = nullptr;
    }

    return *this;
  }

  ~Optional()
  {
    this->free();
  }

  bool isNull() const
  {
    return this->data == nullptr;
  }

  T &getData() const
  {
    if (this->isNull())
    {
      throw "No data";
    }

    return *this->data;
  }

  operator T() const
  {
    return this->getData();
  }

  void clear()
  {
    if (!this->isNull())
    {
      this->free();
    }
  }
};

#endif