#pragma once

class HierarchyPanel;

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

	//つけられている名前を取得
	const std::string& GetName();

	//Inspector Windowに描画する内容。継承先で何も定義しなくてもOK(なにも表示されないだけ)
	virtual void DrawParams();

	virtual ~IComponent() {
	};

	//components_の中身をHierarchy Panelからのみ直接操作したいため
	friend HierarchyPanel;
	
protected:
	std::string name_ = "";

private:
	eastl::multimap<std::string, eastl::unique_ptr<IComponent>> components_;

	IComponent* parent_ = nullptr;

};

template<class Type, class ...Args>
inline void IComponent::AddComponent(std::string key, Args ...args)
{
	auto itr = components_.insert(eastl::make_pair(key, eastl::move(eastl::make_unique<Type>(args...))));
	itr->second->name_ = itr->first;
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

	size_t count = components_.count(key);
	auto itr = components_.find(key);
	for (size_t i = 0; i < count; i++)
	{
		hitComponents.emplace_back(dynamic_cast<Type*>(itr->second.get()));
		itr++;
	}

	return hitComponents;
}