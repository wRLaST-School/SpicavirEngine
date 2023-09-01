#pragma once
class BTENode;

namespace BT {
    class BehaviorTree;

    enum class Status {
        Success,
        Failure,
        Running,
        Completed,
        Error
    };

    class INode {
    protected:
        friend BTENode;

        std::list<std::unique_ptr<INode>> children_;

        uint32_t activeIndex = 0;

        BehaviorTree* parentBT_ = nullptr;
        INode* parent_ = nullptr;
        std::string param_;
    public:
        virtual void OnStart() = 0;
        virtual BT::Status Update() = 0;
        virtual void OnEnd() = 0;
        virtual void OnAbort();

        virtual void SetParam(std::string param);

        std::string GetParam();
        INode* GetParent();
        const std::list<std::unique_ptr<INode>>& GetChildren();

        virtual std::string GetNodeType() = 0;

        //BTEditor用
        BTENode* editorNodePtr = nullptr;
    public:
        template<class NodeType>
        void AddNode(const std::string& param)
        {
            children_.emplace_back(std::make_unique<NodeType>());
            Last()->SetParam(param);
            Last()->parentBT_ = parentBT_;
            Last()->parent_ = this;
        };

        INode* Last();
        std::unique_ptr<INode>* LastPtr();

        //新しいNodeのuptrへのポインタを返す
        std::unique_ptr<INode>* ChangeParent(INode* newParent);

        INode() {};
        virtual ~INode() {};
    };
}

