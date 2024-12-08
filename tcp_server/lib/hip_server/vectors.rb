# frozen_string_literal: true

module HipServer
  # Vector test operation model
  class Vectors
    attr_reader :action, :uuid, :data, :file_path, :response, :record_type

    def initialize(request:, response: "OK")
      @uuid = SecureRandom.uuid
      @request = request
      @response = response
      @action = "vector_ops_test"
      @session_path = File.join(__dir__, "../../db_files/#{$session_uuid}/#{@action}/")
    end

    def save
      @data = CBOR.decode(@request)
      file_save
    rescue StandardError => e
      @record_type = "ERROR"
      @response = "ERROR #{e.message}"
      @data = e.message
    ensure
      db_save
    end

    private

    def dataset
      {
        uuid: @uuid,
        session_uuid: $session_uuid,
        action: @action,
        record_type: @record_type || "FILE",
        data: @data,
        response: @response
      }
    end

    def db_save
      $db.create_data(**dataset)
    end

    def file_save
      FileUtils.mkdir_p @session_path
      @file_path = File.join(@session_path, "#{Time.now.to_i}-#{@uuid}.cbor")
      FileUtils.touch @file_path
      File.open(@file_path, "wb") { |file| file.write(@data) }
      @data = @file_path
    end
  end
end
