#include <iostream>
#include <smk/Color.hpp>
#include <smk/Window.hpp>
#include <smkflow/Elements.hpp>
#include <smkflow/Model.hpp>
#include <smkflow/Widget.hpp>

#include "asset.hpp"

enum Node {
  Number,
  Add,
  Substract,
  Multiply,
  Divide,
};

auto type_number = glm::vec4(1.f, 0.5f, 0.5f, 1.f);
auto node_color = glm::vec4(1.0f, 1.f, 1.f, 1.f);
auto node_color_number = glm::vec4(1.0, 0.5, 0.5, 1.f);

auto node_add = smkflow::model::Node{
    Node::Add,
    "Add",
    node_color,
    {
        {"", type_number},
        {"", type_number},
    },
    {
        smkflow::InputBox::Create(0.f),
        smkflow::InputBox::Create(0.f),
    },
    {
        {"out", type_number},
    },
};

auto node_substract = smkflow::model::Node{
    Node::Substract,
    "Substract",
    node_color,
    {
        {"", type_number},
        {"", type_number},
    },
    {
        smkflow::InputBox::Create(0.f),
        smkflow::InputBox::Create(0.f),
    },
    {
        {"out", type_number},
    },
};

auto node_multiply = smkflow::model::Node{
    Node::Multiply,
    "Multiply",
    node_color,
    {
        {"", type_number},
        {"", type_number},
    },
    {
        smkflow::InputBox::Create(0.f),
        smkflow::InputBox::Create(0.f),
    },
    {
        {"out", type_number},
    },
};

auto node_divide = smkflow::model::Node{
    Node::Divide,
    "Divide",
    node_color,
    {
        {"", type_number},
        {"", type_number},
    },
    {
        smkflow::InputBox::Create(0.f),
        smkflow::InputBox::Create(0.f),
    },
    {
        {"", type_number},
    },
};

auto node_number = smkflow::model::Node{
    Node::Number,
    "Number",
    node_color_number,
    {},
    {
        smkflow::InputBox::Create(0.f),
    },
    {
        {"out", type_number},
    },
};

auto my_board = smkflow::model::Board{
    {
        node_number,
        node_add,
        node_multiply,
        node_substract,
        node_divide,
    },
    asset::arial_ttf,
};

std::map<smkflow::Node*, int> values;

void UpdateValues(smkflow::Board* board) {
  for (int i = 0; i < board->NodeCount(); ++i) {
    smkflow::Node* node = board->NodeAt(i);
    int value = 0;
    if (node->Identifier() == Number) {
      smkflow::InputBox* input = smkflow::InputBox::From(node->WidgetAt(0));
      value = std::stoi(input->GetValue());
      values[node] = value;
      node->OutputAt(0)->SetText(std::to_string(value));
      continue;
    }

    smkflow::InputBox* input_1 = smkflow::InputBox::From(node->WidgetAt(0));
    smkflow::InputBox* input_2 = smkflow::InputBox::From(node->WidgetAt(1));

    if (auto* a = node->InputAt(0)->OppositeNode()) 
      input_1->SetValue(std::to_string(values[a]));

    if (auto* b = node->InputAt(1)->OppositeNode()) 
      input_2->SetValue(std::to_string(values[b]));


    int a_value = std::stoi(input_1->GetValue());
    int b_value = std::stoi(input_2->GetValue());

    // clang-format off
    switch (node->Identifier()) {
      case Number: break;
      case Add: value = a_value + b_value; break;
      case Substract: value = a_value - b_value; break;
      case Multiply: value = a_value * b_value; break;
      case Divide: value = b_value ? a_value / b_value : 0; break;
    }
    // clang-format on
    values[node] = value;
    node->OutputAt(0)->SetText(std::to_string(value));
  }
}

int main() {
  auto window = smk::Window(512, 512, "test");
  auto board = smkflow::Board::Create(my_board);

  // Instanciate some Node based on the model.
  int x = -my_board.nodes.size() / 2;
  for (const auto& node_model : my_board.nodes) {
    for (int y = -4; y < 4; ++y) {
      smkflow::Node* node = board->Create(node_model);
      node->SetPosition({200 * x, 200 * y});
    }
    ++x;
  }

  window.ExecuteMainLoop([&] {
    window.Clear({0.2, 0.2, 0.2, 1.0});
    window.PoolEvents();
    UpdateValues(board.get());
    board->Step(&window, &window.input());
    board->Draw(&window);
    window.Display();
  });
  return EXIT_SUCCESS;
}
