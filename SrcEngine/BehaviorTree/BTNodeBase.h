#pragma once
namespace BT {
    class BehaviorTree;

    enum class Status {
        Success,
        Failure,
        Running,
        Completed
    };

    class INode {
    protected:
        std::vector<std::unique_ptr<INode>> children_;

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
        const std::vector<std::unique_ptr<INode>>& GetChildren();

        virtual std::string GetNodeType() = 0;
    public:
        template<class NodeType>
        void AddNode(const std::string& param)
        {
            children_.push_back(std::move(std::make_unique<NodeType>()));
            Last()->SetParam(param);
            Last()->parentBT_ = parentBT_;
            Last()->parent_ = this;
        };

        INode* Last();

        void ChangeParent(INode* newParent);

        INode() {};
        virtual ~INode() {};
    };
}

