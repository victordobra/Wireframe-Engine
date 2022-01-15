#pragma once
#include <stdexcept>

namespace mge {
	template<class T>
	class vector {
	public:
		template<class T>
		class iterator {
		public:
			iterator() = default;
			iterator(const iterator<T>&) = default;
			iterator(iterator<T>&&) noexcept = default;
			iterator(T* pointer) : pointer(pointer) { }

			iterator<T>& operator=(iterator<T> other) {
				pointer = other.pointer;
				return *this;
			}

			bool operator==(iterator<T> other) const { return pointer == other.pointer; }
			bool operator!=(iterator<T> other) const noexcept { return pointer != other.pointer; }
			bool operator>(iterator<T> other) const { return pointer > other.pointer; }
			bool operator<(iterator<T> other) const { return pointer < other.pointer; }
			bool operator>=(iterator<T> other) const { return pointer >= other.pointer; }
			bool operator<=(iterator<T> other) const { return pointer <= other.pointer; }

			iterator& operator+=(T* other) { pointer += other; }
			iterator& operator+=(size_t other) { pointer += (T*)other; }

			iterator& operator++() { pointer++; }

			~iterator() = default;
		private:
			T* pointer;
		};

		vector() = default;
		vector(const vector<T>& other) : _size(other._size), _data(new T[_size]) {
			for (size_t i = 0; i < _size; i++)
				_data[i] = other._data[i];
		}
		vector(vector<T>&& other) noexcept : _size(other._size), _data(other._data) { other._data = nullptr; }
		explicit vector(size_t mSize) : _size(mSize), _data(new T[mSize]) { }

		vector<T>& operator=(const vector<T>& other) {
			if (&other == this)
				return this;

			//Delete the data and create a new one with the new length
			delete[] _data;
			_size = other._size;
			_data = new T[_size];

			for (size_t i = 0; i < _size; i++)
				_data[i] = other._data[i];

			return this;
		}
		vector<T>& operator=(vector<T>&& other) noexcept {
			if (&other == this)
				return this;

			//Replace the data with the other vector's data and remove the other vector's data
			_size = other._size;
			_data = other._data;

			other._data = nullptr;

			return this;
		}

		bool operator==(const vector<T>&) const = delete;
		bool operator==(vector<T>&&) const noexcept = delete;
		bool operator!=(const vector<T>&) const = delete;
		bool operator!=(vector<T>&&) const noexcept = delete;

		const T& operator[](size_t i) const { return _data[i]; }
		T& operator[](size_t i) { return _data[i]; }

		size_t size() { return _size; }
		T* data() { return _data; }

		void push_back(const T& newObject) {
			//Create a new data array
			_size++;
			T* newData = new T[_size];

			//Copy everything over
			for (size_t i = 0; i < _size - 1; i++)
				newData[i] = _data[i];

			//Add the new item
			newData[_size - 1] = newObject;

			//Delete the old data array and replace it with the new one
			delete[] _data;
			_data = newData;
		}
		void push_back(T&& newObject) {
			//Create a new data array
			_size++;
			T* newData = new T[_size];

			//Copy everything over
			for (size_t i = 0; i < _size - 1; i++)
				newData[i] = _data[i];

			//Add the new item
			newData[_size - 1] = newObject;

			//Delete the old data array and replace it with the new one
			delete[] _data;
			_data = newData;
		}
		void pop() {
			//Create a new data array
			_size--;
			T* newData = new T[_size];

			//Copy everything over
			for (size_t i = 0; i < _size; i++)
				newData[i] = _data[i];

			//Delete the old data array and replace it with the new one
			delete[] _data;
			_data = newData;
		}

		iterator<T> begin() { return _data; }
		iterator<T> end() { return _data + _size; }

		void erase(iterator<T> left, iterator<T> right) {
			//Check if every item is in range
			if (left < begin() || left > end() || right < begin() || right < end())
				throw std::runtime_error("Left and right should be in range!");

			size_t dataLeft = left - _data;
			size_t dataRight = right - _data;

			//Create a new data array
			size_t oldSize = _size;
			_size -= dataRight - dataLeft;
			T* newData = new T[_size];

			//Copy the remaining items over
			size_t newIndex = 0;
			for (size_t i = 0; i < dataLeft; i++) {
				newData[newIndex] = _data[i];
				newIndex++;
			}
			for (size_t i = dataRight; i < oldSize; i++) {
				newData[newIndex] = _data[i];
				newIndex++;
			}

			//Delete the old data array and replace it with the new one
			delete[] _data;
			_data = newData;
		}

		~vector() { delete[] _data; }
	private:
		size_t _size;
		T* _data;
	};
}
