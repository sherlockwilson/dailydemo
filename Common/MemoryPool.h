#pragma once

#include <climits>
#include <cstddef>

template <typename T, size_t BlockSize = 4096>

class MemoryPool : public std::allocator<T>
{
public:
	// ʹ�� typedef ��������д
	typedef T*              pointer;

	// ���� rebind<U>::other �ӿ�
	template <typename U> struct rebind {
		typedef MemoryPool<U> other;
	};

	// Ĭ�Ϲ���
	// C++11 ʹ���� noexcept ����ʽ�������˺��������׳��쳣
	MemoryPool() {
		currentBlock_ = nullptr;
		currentSlot_ = nullptr;
		lastSlot_ = nullptr;
		freeSlots_ = nullptr;
	}

	// ����һ�����е��ڴ��
	~MemoryPool() {
		// ѭ�������ڴ���з�����ڴ�����
		slot_pointer_ curr = currentBlock_;
		while (curr != nullptr) {
			slot_pointer_ prev = curr->next;
			operator delete(reinterpret_cast<void*>(curr));
			curr = prev;
		}
	}

	// ͬһʱ��ֻ�ܷ���һ������, n �� hint �ᱻ����
	pointer allocate(size_t n = 1, const T* hint = 0) {
		if (freeSlots_ != nullptr) {
			pointer result = reinterpret_cast<pointer>(freeSlots_);
			freeSlots_ = freeSlots_->next;
			return result;
		}
		else {
			if (currentSlot_ >= lastSlot_) {
				// ����һ���ڴ�����
				data_pointer_ newBlock = reinterpret_cast<data_pointer_>(operator new(BlockSize));
				reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
				currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);
				data_pointer_ body = newBlock + sizeof(slot_pointer_);
				uintptr_t result = reinterpret_cast<uintptr_t>(body);
				size_t bodyPadding = (__alignof(slot_type_)-result) % __alignof(slot_type_);
				currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
				lastSlot_ = reinterpret_cast<slot_pointer_>(newBlock + BlockSize - sizeof(slot_type_) + 1);
			}
			return reinterpret_cast<pointer>(currentSlot_++);
		}
	}

	// ����ָ�� p ָ����ڴ�����
	void deallocate(pointer p, size_t n = 1) {
		if (p != nullptr) {
			reinterpret_cast<slot_pointer_>(p)->next = freeSlots_;
			freeSlots_ = reinterpret_cast<slot_pointer_>(p);
		}
	}

	// ���ù��캯��, ʹ�� std::forward ת�����ģ��
	template <typename U, typename... Args>
	void construct(U* p, Args&&... args) {
		new (p)U(std::forward<Args>(args)...);
	}

	// �����ڴ���еĶ���, �����ö������������
	template <typename U>
	void destroy(U* p) {
		p->~U();
	}

private:
	// ���ڴ洢�ڴ���еĶ����
	union Slot_ {
		T element;
		Slot_* next;
	};

	// ����ָ��
	typedef char* data_pointer_;
	// �����
	typedef Slot_ slot_type_;
	// �����ָ��
	typedef Slot_* slot_pointer_;

	// ָ��ǰ�ڴ�����
	slot_pointer_ currentBlock_;
	// ָ��ǰ�ڴ������һ�������
	slot_pointer_ currentSlot_;
	// ָ��ǰ�ڴ���������һ�������
	slot_pointer_ lastSlot_;
	// ָ��ǰ�ڴ������еĿ��ж����
	slot_pointer_ freeSlots_;
	// ��鶨����ڴ�ش�С�Ƿ��С
	static_assert(BlockSize >= 2 * sizeof(slot_type_), "BlockSize too small.");
};