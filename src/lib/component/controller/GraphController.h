#ifndef GRAPH_CONTROLLER_H
#define GRAPH_CONTROLLER_H

#include <list>
#include <vector>

#include "utility/messaging/MessageListener.h"
#include "utility/messaging/type/MessageActivateAll.h"
#include "utility/messaging/type/MessageActivateTokens.h"
#include "utility/messaging/type/MessageColorSchemeTest.h"
#include "utility/messaging/type/MessageFlushUpdates.h"
#include "utility/messaging/type/MessageFocusIn.h"
#include "utility/messaging/type/MessageFocusOut.h"
#include "utility/messaging/type/MessageGraphNodeBundleSplit.h"
#include "utility/messaging/type/MessageGraphNodeExpand.h"
#include "utility/messaging/type/MessageGraphNodeMove.h"
#include "utility/messaging/type/MessageSearchFullText.h"
#include "utility/messaging/type/MessageShowErrors.h"
#include "utility/messaging/type/MessageShowReference.h"

#include "component/controller/Controller.h"
#include "component/controller/helper/DummyEdge.h"
#include "component/controller/helper/DummyNode.h"
#include "component/view/GraphView.h"
#include "data/graph/Node.h"
#include "data/graph/token_component/TokenComponentAccess.h"
#include "data/graph/token_component/TokenComponentAggregation.h"

class Graph;
class StorageAccess;

class GraphController
	: public Controller
	, public MessageListener<MessageActivateAll>
	, public MessageListener<MessageActivateTokens>
	, public MessageListener<MessageColorSchemeTest>
	, public MessageListener<MessageFlushUpdates>
	, public MessageListener<MessageFocusIn>
	, public MessageListener<MessageFocusOut>
	, public MessageListener<MessageGraphNodeBundleSplit>
	, public MessageListener<MessageGraphNodeExpand>
	, public MessageListener<MessageGraphNodeMove>
	, public MessageListener<MessageSearchFullText>
	, public MessageListener<MessageShowErrors>
	, public MessageListener<MessageShowReference>
{
public:
	GraphController(StorageAccess* storageAccess);
	~GraphController();

private:
	virtual void handleMessage(MessageActivateAll* message);
	virtual void handleMessage(MessageActivateTokens* message);
	virtual void handleMessage(MessageFlushUpdates* message);
	virtual void handleMessage(MessageFocusIn* message);
	virtual void handleMessage(MessageFocusOut* message);
	virtual void handleMessage(MessageGraphNodeBundleSplit* message);
	virtual void handleMessage(MessageGraphNodeExpand* message);
	virtual void handleMessage(MessageGraphNodeMove* message);
	virtual void handleMessage(MessageSearchFullText* message);
	virtual void handleMessage(MessageShowErrors* message);
	virtual void handleMessage(MessageShowReference* message);

	GraphView* getView() const;

	virtual void clear();

	void createDummyGraphForTokenIds(const std::vector<Id>& tokenIds, const std::shared_ptr<Graph> graph);
	void createDummyGraphForTokenIdsAndSetActiveAndVisibility(
		const std::vector<Id>& tokenIds, const std::shared_ptr<Graph> graph);
	std::shared_ptr<DummyNode> createDummyNodeTopDown(Node* node, Id parentId);

	std::vector<Id> getExpandedNodeIds() const;
	void setExpandedNodeIds(const std::vector<Id>& nodeIds);
	void autoExpandActiveNode(const std::vector<Id>& activeTokenIds);

	bool setActive(const std::vector<Id>& activeTokenIds);
	void setVisibility(bool noActive);
	void setActiveAndVisibility(const std::vector<Id>& activeTokenIds);
	void setNodeActiveRecursive(DummyNode* node, const std::vector<Id>& activeTokenIds, bool* noActive) const;
	void removeImplicitChildrenRecursive(DummyNode* node);
	bool setNodeVisibilityRecursiveBottomUp(DummyNode* node, bool noActive) const;
	void setNodeVisibilityRecursiveTopDown(DummyNode* node, bool parentExpanded) const;

	void bundleNodes();
	void bundleNodesAndEdgesMatching(
		std::function<bool(const DummyNode::BundleInfo&, const Node*)> matcher, size_t count, const std::string& name);
	std::shared_ptr<DummyNode> bundleNodesMatching(
		std::list<std::shared_ptr<DummyNode>>& nodes, std::function<bool(const DummyNode*)> matcher, const std::string& name);
	void bundleByType(std::list<std::shared_ptr<DummyNode>>& nodes, Node::NodeType type, const std::string& name);
	void bundleNodesByType();

	void addCharacterIndex();

	void layoutNesting();
	void layoutNestingRecursive(DummyNode* node) const;
	void addExpandToggleNode(DummyNode* node) const;
	void layoutToGrid(DummyNode* node) const;

	void layoutGraph(bool getSortedNodes = false);
	void layoutList();

	void assignBundleIds();

	DummyNode* getDummyGraphNodeById(Id tokenId) const;

	void buildGraph(MessageBase* message, bool centerActiveNode, bool animatedTransition = true, bool scrollToTop = false);

	void forEachDummyNodeRecursive(std::function<void(DummyNode*)> func);
	void forEachDummyEdge(std::function<void(DummyEdge*)> func);

	virtual void handleMessage(MessageColorSchemeTest* message);

	StorageAccess* m_storageAccess;

	std::vector<std::shared_ptr<DummyNode>> m_dummyNodes;
	std::vector<std::shared_ptr<DummyEdge>> m_dummyEdges;

	std::map<Id, std::shared_ptr<DummyNode>> m_dummyGraphNodes;

	std::vector<Id> m_activeNodeIds;
	std::vector<Id> m_activeEdgeIds;

	std::shared_ptr<Graph> m_graph;
};

#endif // GRAPH_CONTROLLER_H
