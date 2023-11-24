#pragma once

#pragma warning (push)
#pragma warning (disable:26800)
#include <SrcExternal/json.hpp>
#pragma warning (pop)
/*
-------------------------------------------------------------------------------
コンポーネントを作る際は下記2点を行うこと
・ファクトリーからこのクラスのインスタンスを作るための関数を継承先で作成
・ComponentFactory::Registerでクラス名とCreate関数を紐づける

以下のマクロをクラス内でメンバとして書き、CustomComponentRegisterer.cppで
クラス名::RegisterToComponentFactory()を呼ぶことで上記を行うことも可能。
ComponentFactory.hをインクルードすること。
-------------------------------------------------------------------------------
*/

#define ComponentFactoryRegister(className) static eastl::unique_ptr<className> Create(){\
	return eastl::make_unique<className>();\
};\
static void RegisterToComponentFactory() {\
	ComponentFactory::Register(#className, className::Create);\
}\
std::string GetClassString() override {\
	return std::string(#className);\
}\

class HierarchyPanel;

class IComponent
{
public:
	//指定したキーのコンポーネントを指定したクラスで作成
	//指定したクラスのコンストラクタの引数を取る
	template <class Type, class ...Args>
	Type* AddComponent(std::string key, Args ...args);

	IComponent* AddComponent(std::string key, eastl::unique_ptr<IComponent> component);

	//コンポーネントの親を変更
	void ChangeParent(IComponent* newParent);

	//指定したキーのコンポーネントを一つ削除
	//該当要素が複数ある場合の動作は保証しない
	void RemoveComponent(std::string key);

	//ポインタが指すコンポーネントを所持している場合削除する
	void RemoveComponent(IComponent* ptr);

	//指定したキーのコンポーネントを全て削除
	void ClearComponentWithKey(std::string key);

	//全てのコンポーネントを削除
	void ClearAllComponents();

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

	//全てのコンポーネントを取得
	const eastl::multimap<std::string, eastl::unique_ptr<IComponent>>& GetAllConponents();

	//つけられている名前を取得
	const std::string& GetName();

	//コンポーネントのクラス名を取得
	virtual std::string GetClassString() = 0;

	//コンポーネント共通で自動で呼ばれる処理
	virtual void Init();
	virtual void Update();
	virtual void Draw();

	static void InitAllChildComponents(IComponent* parent);
	static void UpdateAllChildComponents(IComponent* parent);
	static void DrawAllChildComponents(IComponent* parent);

	//読み書きに使う関数
	/*
	* "ComponentType":"BraBra", 
	* "ComponentParams":[
	*	{
	*		この中の部分を実装
	* 　　 |　ここまでのインデントを引数に取る
	*	}
	* ]
	*/
	virtual void WriteParamJson([[maybe_unused]] nlohmann::json& jsonObj) {};
	/*
	* obj["ComponentParams"]のオブジェクトを受け取る
	*/
	virtual void ReadParamJson([[maybe_unused]]const nlohmann::json& paramsObject) {};

	//Inspector Windowに描画する内容。継承先で何も定義しなくてもOK(なにも表示されないだけ)
	virtual void DrawParams();

	template <class Type>
	Type* CastTo();

	virtual ~IComponent() {
	};

	//components_の中身をHierarchy Panelからのみ直接操作したいため
	friend HierarchyPanel;
	
protected:
	std::string name_ = "";

	eastl::multimap<std::string, eastl::unique_ptr<IComponent>> components_;

private:
	IComponent* parent_ = nullptr;

};

template<class Type, class ...Args>
inline Type* IComponent::AddComponent(std::string key, Args ...args)
{
	auto itr = components_.insert(eastl::make_pair(key, eastl::move(eastl::make_unique<Type>(args...))));
	itr->second->name_ = itr->first;

	return dynamic_cast<Type*>(itr->second.get());
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

template<class Type>
inline Type* IComponent::CastTo()
{
	return dynamic_cast<Type>(this);
}
