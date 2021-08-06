--Root Object
    --clip area 2d 100x100 - chat 
    --chat item 1
    --...
    --chat item n
    --clip area 2d -status
    --dps 1
    --..
    --dps n
--Camera
SceneRoot=Alice.GameObject.New()
CameraObject=Alice.GameObject.New()
Camera=CameraObject:AddComponent(Alice.Camera)
local object=Alice.GameObject.New()
object:SetPosition(0.0,0.0,-5.0)
local mesh=object:AddComponent(Alice.Mesh)
mesh:SetVertexCount(6)
mesh:SetSubMeshCount(2)

mesh:SetSubMeshIndexCount(0,3)
mesh:SetSubMeshIndexCount(1,3)

mesh:UpdatePosition(0,-0.5,-0.5,0.0)
mesh:UpdatePosition(1,0.5,-0.5,0.0)
mesh:UpdatePosition(2,0.0,0.5,0.0)
mesh:UpdatePosition(3,1.0-0.5,-0.5,0.0)
mesh:UpdatePosition(4,1.0+0.5,-0.5,0.0)
mesh:UpdatePosition(5,1.0+0.0,0.5,0.0)

mesh:UpdateSubMeshIndex(0,0,0)
mesh:UpdateSubMeshIndex(0,1,1)
mesh:UpdateSubMeshIndex(0,2,2)

mesh:UpdateSubMeshIndex(1,0,3)
mesh:UpdateSubMeshIndex(1,1,4)
mesh:UpdateSubMeshIndex(1,2,5)

local mesh_renderer=object:AddComponent(Alice.MeshRenderer)
mesh_renderer:SetMaterialCount(2)
local material=Alice.Material.Clone("builtin/Material/SimpleColor")
mesh_renderer:SetMaterial(0,material)

material=Alice.Material.Clone("builtin/Material/SimpleColor")
material:SetVec4("U_Color",1.0,0.0,0.0,1.0)
mesh_renderer:SetMaterial(1,material)