#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

//#include "node.hpp"
#include "geometry_node.hpp"
#include "camera_node.hpp"

TEST_CASE("node","[node]") {

	SECTION("constructor, getter") {
		mat = glm::mat4(1.0);
		
		Node node1{nullptr, "node1", mat, mat};
		Node node2{node1, "node2", mat, mat};
		Node node3{nullptr, "node3", mat, mat};
		
		REQUIRE(node1.getParent() == nullptr);
		REQUIRE(node2.getParent() == node1);
		REQUIRE(node3.setParent(node2).getParent() == node2);

		REQUIRE(node1.getChildren(node2) == node2);
		REQUIRE(node2.getChildren(node3) == node3);

		REQUIRE(node1.getChildrenList().begin() == node2);
		REQUIRE(node2.getChildrenList().begin() == node3);

		REQUIRE(node1.getName() == "node1");
		REQUIRE(node2.getName() == "node2");
		REQUIRE(node3.getName() == "node3");
	}
}


int main(int argc, char *argv[])
{
  return Catch::Session().run(argc, argv);
}