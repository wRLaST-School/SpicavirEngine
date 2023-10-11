#pragma once
class IComponent
{
public:

	//指定したキーのコンポーネントを指定したクラスで作成
	//指定したクラスのコンストラクタの引数を取る
	template <class Type, class ...Args>
	void AddComponent(std::string key, Args ...args);

	//指定したキーのコンポーネントを一つ削除
	//該当要素が複数ある場合の動作は保証しない
	void RemoveComponent(std::string key);

	//ポインタが指すコンポーネントを所持している場合削除する
	void RemoveComponent(IComponent* ptr);

	//指定したキーのコンポーネントを全て削除
	void ClearComponentWithKey(std::string key);

	//指定したキーのコンポーネントのポインタを一つ取得
	//該当要素が複数ある場合の動作は保証しない
	IComponent* GetComponent(std::string key);
	
	//指定したキーのコンポーネントをTypeで指定した型のポインタにして一つ取得
	//該当要素が複数ある場合の動作は保証しない
	template <class Type> Type* GetComponent(std::string key);

	//指定したキーに該当する全てのコンポーネントのポインタをリストにして取得
	eastl::list<IComponent*> GetComponents(std::string key);

	//指定したキーに該当する全てのコンポーネントをTypeで指定した型のポインタのリストにして取得
	template <class Type> eastl::list<Type*> GetComponents(std::string key);

private:
	eastl::hash_multimap<std::string, std::unique_ptr<IComponent>> components_;

	IComponent* parent_ = nullptr;
};

template<class Type, class ...Args>
inline void IComponent::AddComponent(std::string key, Args ...args)
{
	components_.insert(eastl::make_pair(key, std::move(std::make_unique<Type>(args...))));
}

template<class Type>
inline Type* IComponent::GetComponent(std::string key)
{
	return dynamic_cast<Type*>(components_.find(key)->second.get());
}

template<class Type>
inline eastl::list<Type*> IComponent::GetComponents(std::string key)
{
	eastl::list<Type*> hitComponents;

	int32_t count = components_.count(key);
	auto itr = components_.find(key);
	for (int32_t i = 0; i < count; i++)
	{
		hitComponents.emplace_back(dynamic_cast<Type*>(itr->second.get()));
	}

	return hitComponents;
}