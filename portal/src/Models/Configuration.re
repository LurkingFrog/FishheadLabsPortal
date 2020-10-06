/** User settings stored in the database.
 *
 * These are the options relating to managing the user experience of the portal
 */
/**
 * Available widgets that can be added to the dashboard
 */
module Widget = {
  type widgetType =
    | Database
    | Task;

  type t = {
    widgetType,
    key: string,
  };
};

module Dashboard = {
  type t = {widgets: array(Widget.t)};
};

type t = {dashboard: Dashboard.t};
let default = () => {
  dashboard: {
    widgets: [|
      {widgetType: Widget.Database, key: Uuid.make()},
      {widgetType: Widget.Task, key: Uuid.make()},
    |],
  },
};

type action =
  | AddDashboardWidget(Widget.t)
  // TODO: Add this. It should be drag and drop on the dashboard page
  // | MoveWidget(string, int)
  | RemoveDashboardWidget(string);

let reducer = (state: t, action) => {
  switch (action) {
  | AddDashboardWidget(widget) => {
      dashboard: {
        widgets: Array.append(state.dashboard.widgets, [|widget|]),
      },
    }
  // | MoveWidget:
  | RemoveDashboardWidget(key) => {
      dashboard: {
        widgets:
          state.dashboard.widgets
          |> Array.to_list
          |> List.filter((widget: Widget.t) => key != widget.key)
          |> Array.of_list,
      },
    }
  };
};
