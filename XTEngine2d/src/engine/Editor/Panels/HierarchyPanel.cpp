#include "HierarchyPanel.h"

HierarchyPanel::HierarchyPanel()
{
}

HierarchyPanel::~HierarchyPanel()
{
}

void HierarchyPanel::OnImGuiRender(XTEngine2d::Scene* scene, Entity& m_SelectedEntity)
{
	auto entities = scene->m_Registry.GetAllEntities();
    DrawHierarchy(entities, scene, m_SelectedEntity);
}

void HierarchyPanel::DrawHierarchy(std::vector<Entity>& entities,XTEngine2d::Scene* scene, Entity& m_SelectedEntity)
{
    ImGui::Begin("Hierarchy");

    for (auto entity : entities)
    {
        bool isRoot = true;

        if (scene->m_Registry.HasComponent<Hierarchy>(entity))
        {
            auto& hierarchy =
                scene->m_Registry.GetComponent<Hierarchy>(entity);

            isRoot = hierarchy.parent == 0;
        }

        if (isRoot)
        {
            DrawEntityNode(entity, scene, m_SelectedEntity);
        }
    }

    ImGui::End();
}


void HierarchyPanel::DrawEntityNode(Entity entity, XTEngine2d::Scene* scene, Entity& m_SelectedEntity)
{

    if (!scene->m_Registry.HasComponent<XTEngine2d::Tag>(entity))
        return;

    auto& tag =
        scene->m_Registry.GetComponent<XTEngine2d::Tag>(entity);

    ImGuiTreeNodeFlags flags =
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    if (m_SelectedEntity == entity)
        flags |= ImGuiTreeNodeFlags_Selected;

    bool hasChildren = false;

    if (scene->m_Registry.HasComponent<Hierarchy>(entity))
    {
        auto& hierarchy =
            scene->m_Registry.GetComponent<Hierarchy>(entity);

        hasChildren = !hierarchy.children.empty();
    }

    if (!hasChildren)
        flags |= ImGuiTreeNodeFlags_Leaf;

    bool opened = ImGui::TreeNodeEx(
        (void*)(uint64_t)entity,
        flags,
        tag.name.c_str());

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("HIERARCHY_ENTITY", &entity, sizeof(entity));

        if (scene->m_Registry.HasComponent<XTEngine2d::Tag>(m_SelectedEntity))
        {
            XTEngine2d::Tag tag = scene->m_Registry.GetComponent<XTEngine2d::Tag>(m_SelectedEntity);
            ImGui::Text("%s", tag.name);
        }
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload =
            ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
        {
            Entity dragged =
                *(Entity*)payload->Data;

            if (dragged != entity)
            {
                ReparentEntity(
                    dragged,
                    entity,
                    scene);
            }
        }

        ImGui::EndDragDropTarget();
    }



    if (ImGui::IsItemClicked())
    {
        m_SelectedEntity = entity;
    }

    if (opened)
    {
        if (hasChildren)
        {
            auto& hierarchy =
                scene->m_Registry.GetComponent<Hierarchy>(entity);

            for (auto child : hierarchy.children)
            {
                DrawEntityNode(child, scene, m_SelectedEntity);
            }
        }

        ImGui::TreePop();
    }

   

}

void HierarchyPanel::ReparentEntity( Entity child, Entity newParent, XTEngine2d::Scene * scene)
{
    if (child == newParent)
        return;

    // Child hierarchy
    if (!scene->m_Registry.HasComponent<Hierarchy>(child))
    {
        scene->m_Registry.AddComponent(
            child,
            Hierarchy{});
    }

    auto& childHierarchy =
        scene->m_Registry.GetComponent<Hierarchy>(child);

    // Remove from old parent
    if (childHierarchy.parent != 0)
    {
        Entity oldParent =
            childHierarchy.parent;

        if (scene->m_Registry.HasComponent<Hierarchy>(oldParent))
        {
            auto& oldHierarchy =
                scene->m_Registry.GetComponent<Hierarchy>(oldParent);

            auto& children =
                oldHierarchy.children;

            children.erase(
                std::remove(
                    children.begin(),
                    children.end(),
                    child),
                children.end());
        }
    }

    // Assign new parent
    childHierarchy.parent = newParent;

    // Ensure parent hierarchy exists
    if (!scene->m_Registry.HasComponent<Hierarchy>(newParent))
    {
        scene->m_Registry.AddComponent(
            newParent,
            Hierarchy{});
    }

    // Add child to new parent
    scene->m_Registry
        .GetComponent<Hierarchy>(newParent)
        .children
        .push_back(child);
}

bool HierarchyPanel::IsChildOf(Entity entity, Entity possibleParent, XTEngine2d::Scene* scene)
{
    if (!scene->m_Registry.HasComponent<Hierarchy>(entity))
        return false;

    auto& hierarchy =
        scene->m_Registry.GetComponent<Hierarchy>(entity);

    if (hierarchy.parent == possibleParent)
        return true;

    if (hierarchy.parent == 0)
        return false;

    return IsChildOf(
        hierarchy.parent,
        possibleParent,
        scene);
}