#pragma once
namespace BT {
    enum class Status {
        Inactive,
        Success,
        Failure,
        Running
    };

    class INode {
    private:
        std::vector<INode*> children_;
        BT::Status status_ = Status::Inactive;

    protected:
        virtual void OnStart() = 0;
        virtual BT::Status OnUpdate() = 0;
        virtual void OnEnd() = 0;
        virtual void OnAbort();

        virtual void SetParam(std::string param) = delete;

        template<class NodeType>
        void AddNode(std::string param)
        {
            children_.emplace_back();
            Last().SetParam(param);
        };

        INode* Last();

    public:
        INode() {};
        virtual ~INode() = delete;
    };
}

